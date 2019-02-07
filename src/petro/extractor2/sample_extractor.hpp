// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <memory>

#include "../parser.hpp"
#include "../box/moov.hpp"
#include "../box/trak.hpp"
#include "../box/mdia.hpp"
#include "../box/hdlr.hpp"
#include "../box/mdhd.hpp"
#include "../box/mvhd.hpp"
#include "../box/minf.hpp"
#include "../box/stbl.hpp"
#include "../box/stco.hpp"
#include "../box/stsd.hpp"
#include "../box/co64.hpp"
#include "../box/stsz.hpp"
#include "../box/stsc.hpp"
#include "../box/ctts.hpp"
#include "../box/stts.hpp"
#include "../box/stsz.hpp"
#include "../box/stbl.hpp"
#include "../box/root.hpp"

#include "track_type.hpp"
#include "../decoding_time.hpp"
#include "../presentation_time.hpp"

namespace petro
{
namespace extractor
{
class sample_extractor
{
public:

    virtual void open(
        const uint8_t* data,
        uint64_t size,
        uint32_t track_id,
        std::error_code& error)
    {
        parser<
            box::moov<parser<
                box::mvhd,
                box::trak<parser<
                    box::mdia<parser<
                        box::hdlr,
                        box::mdhd,
                        box::minf<parser<
                            box::stbl<parser<
                                box::stco,
                                box::stsc,
                                box::stsd,
                                box::co64,
                                box::ctts,
                                box::stts,
                                box::stsz
                            >>
                        >>
                    >>
                >>
            >>
        > parser;
        auto root = std::make_shared<petro::box::root>();
        parser.parse(data, size, root, error);
        if (error)
            return;

        std::shared_ptr<box::box> trak;
        for (auto tkhd : m_root->template get_children<box::tkhd>())
        {
            if (tkhd->track_id() == track_id)
            {
                trak = tkhd->parent();
                break;
            }
        }
        if (trak == nullptr)
        {
            error = petro::error::trak_box_missing;
            return;
        }

        auto stco = trak->template get_child<box::stco>();
        if (stco == nullptr)
        {
            auto co64 = trak->template get_child<box::co64>();
            if (co64 == nullptr)
            {
                error = petro::error::stco_box_missing;
                // or co64 box...
                return;
            }
        }

        auto stsc = trak->template get_child<box::stsc>();
        if (stsc == nullptr)
        {
            error = petro::error::stsc_box_missing;
            return;
        }

        auto stsz = trak->template get_child<box::stsz>();
        if (stsz == nullptr)
        {
            error = petro::error::stsz_box_missing;
            return;
        }

        auto stsd = trak->template get_child<box::stsd>();
        if (stsd == nullptr)
        {
            error = petro::error::stsd_box_missing;
            return;
        }

        auto mdhd = trak->template get_child<box::mdhd>();
        if (mdhd == nullptr)
        {
            error = petro::error::mdhd_box_missing;
            return;
        }

        auto stts = trak->template get_child<box::stts>();
        if (stts == nullptr)
        {
            error = petro::error::stts_box_missing;
            return;
        }

        reset();
        m_root = root;
        m_trak = trak;
        m_stsc = stsc;
        m_stsz = stsz;

        m_data = data;
        m_size = size;

        m_stsd = stsd;

        m_mdhd = mdhd;
        m_stts = stts;
        m_ctts = trak->template get_child<box::ctts>();
    }

    void close()
    {
        m_root = nullptr;
    }

    /// Reset this layer
    void reset()
    {
        m_sample_index = 0;
        m_chunk_index = 0;
        m_chunk_sample = 0;
        m_offset = 0;
    }

    /// Advance to next sample
    void advance()
    {
        assert(m_root != nullptr);
        assert(!at_end());
        m_offset += sample_size();
        m_sample_index += 1;
        m_chunk_sample += 1;
        if (m_chunk_sample >= samples_for_chunk())
        {
            m_chunk_index += 1;
            m_chunk_sample = 0;
            m_offset = 0;
        }
    }

    /// Return true if no more sample are available.
    bool at_end() const
    {
        assert(m_root != nullptr);
        return (m_sample_index >= samples());
    }

    uint32_t samples() const
    {
        assert(m_root != nullptr);
        return m_stsz->sample_count();
    }

    /// Return a pointer to the sample data
    const uint8_t* sample_data() const
    {
        assert(m_root != nullptr);
        assert(!at_end());
        return m_data + chunk_offsets()[m_chunk_index] + m_offset;
    }

    /// Return the size of the sample data
    uint32_t sample_size() const
    {
        assert(m_root != nullptr);
        assert(!at_end());
        assert(m_stsz != nullptr);
        return m_stsz->sample_size(m_sample_index);
    }

    /// Return the current sample index
    uint32_t sample_index() const
    {
        assert(m_root != nullptr);
        assert(!at_end());
        return m_sample_index;
    }

    /// Return the decoding timestamp related to the current sample
    uint64_t decoding_timestamp() const
    {
        assert(m_stts != nullptr);
        return decoding_time(m_stts, m_mdhd->timescale(), m_sample_index);
    }

    /// Return the presentation timestamp related to the current sample
    uint64_t presentation_timestamp() const
    {
        assert(m_stts != nullptr);
        return presentation_time(
            m_stts, m_ctts, m_mdhd->timescale(), m_sample_index);
    }

private:

    const std::vector<uint64_t>& chunk_offsets() const
    {
        assert(m_root != nullptr);
        auto stco = m_trak->template get_child<box::stco>();
        if (stco != nullptr)
        {
            return stco->entries();
        }
        else
        {
            auto co64 = m_trak->template get_child<box::co64>();
            assert(co64 != nullptr);
            return co64->entries();
        }
    }

    uint32_t samples_for_chunk()
    {
        assert(m_root != nullptr);
        return m_stsc->samples_for_chunk(m_chunk_index);
    }

private:

    const uint8_t* m_data = nullptr;
    uint64_t m_size = 0;

    uint32_t m_offset = 0;
    uint32_t m_sample_index = 0;
    uint32_t m_chunk_index = 0;
    uint32_t m_chunk_sample = 0;

    std::shared_ptr<const box::box> m_trak;
    std::shared_ptr<const box::stsc> m_stsc;
    std::shared_ptr<const box::stsz> m_stsz;

    std::shared_ptr<const box::root> m_root;

    std::shared_ptr<const box::mdhd> m_mdhd;
    std::shared_ptr<const box::stts> m_stts;
    std::shared_ptr<const box::ctts> m_ctts;

    std::shared_ptr<const box::stsd> m_stsd;
};
}
}
