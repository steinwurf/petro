// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "h264_sample_extractor.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>

#include "../parser.hpp"
#include "../box/moov.hpp"
#include "../box/trak.hpp"
#include "../box/mdia.hpp"
#include "../box/hdlr.hpp"
#include "../box/mdhd.hpp"
#include "../box/minf.hpp"
#include "../box/stbl.hpp"
#include "../box/stco.hpp"
#include "../box/stsd.hpp"
#include "../box/co64.hpp"
#include "../box/stsz.hpp"
#include "../box/root.hpp"

#include "../decoding_time.hpp"
#include "../presentation_time.hpp"

namespace petro
{
namespace extractor
{
    bool h264_sample_extractor::open(const std::string& filename)
    {
        assert(!m_file.is_open() && "Close the extractor before reopening.");
        m_file.open(filename);
        if (!m_file.is_open())
            return false;

        parser<
            box::moov<parser<
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

        auto root = std::make_shared<box::root>();
        byte_stream bs((uint8_t*)m_file.data(), m_file.size());

        parser.read(root, bs);

        // Get the needed boxes
        auto avc1 = root->get_child("avc1");
        if (avc1 == nullptr)
             return false;

        m_avcc = avc1->get_child<box::avcc>();
        if (m_avcc == nullptr)
            return false;

        auto trak = avc1->get_parent("trak");
        if (trak == nullptr)
            return false;

        auto stco = trak->get_child<box::stco>();
        if (stco != nullptr)
        {
            m_chunk_offsets.resize(stco->entry_count());
            std::copy(stco->entries().begin(), stco->entries().end(),
                      m_chunk_offsets.begin());
        }
        else
        {
            auto co64 = trak->get_child<box::co64>();
            if (co64 == nullptr)
                return false;

            m_chunk_offsets.resize(co64->entry_count());
            std::copy(co64->entries().begin(), co64->entries().end(),
                      m_chunk_offsets.begin());
        }

        m_stsc = trak->get_child<box::stsc>();
        assert(m_stsc != nullptr);

        m_stsz = trak->get_child<box::stsz>();
        assert(m_stsz != nullptr);

        auto mdhd = trak->get_child<box::mdhd>();
        assert(mdhd != nullptr);
        m_timescale = mdhd->timescale();

        m_stts = trak->get_child<box::stts>();
        assert(m_stts != nullptr);

        m_ctts = trak->get_child<box::ctts>();

        reset();

        return true;
    }

    void h264_sample_extractor::close()
    {
        assert(is_open());
        m_file.close();
        m_avcc.reset();
        m_stsc.reset();
        m_stsz.reset();
        m_stts.reset();
        m_ctts.reset();
        m_chunk_offsets.clear();
        m_timescale = 0;
        reset();
    }

    void h264_sample_extractor::advance()
    {
        m_sample += 1;
        m_chunk_sample += 1;
        if (m_chunk_sample >= m_stsc->samples_for_chunk(m_chunk_index))
        {
            m_chunk_index += 1;
            m_chunk_sample = 0;
        }
    }

    bool h264_sample_extractor::is_open() const
    {
        return m_file.is_open();
    }

    bool h264_sample_extractor::at_end() const
    {
        assert(m_stsz != nullptr);
        return (m_sample >= m_stsz->sample_count());
    }

    void h264_sample_extractor::reset()
    {
        m_sample = 0;
        m_chunk_index = 0;
        m_chunk_sample = 0;
    }

    const uint8_t* h264_sample_extractor::sample_data() const
    {
        assert(m_file.is_open());
        assert(m_chunk_offsets.size() != 0);
        return ((uint8_t*)m_file.data()) +  m_chunk_offsets[m_chunk_index];
    }

    uint32_t h264_sample_extractor::sample_size() const
    {
        assert(m_stsz != nullptr);
        return m_stsz->sample_size(m_sample);
    }

    uint64_t h264_sample_extractor::timestamp() const
    {
        return presentation_timestamp();
    }

    const uint8_t* h264_sample_extractor::pps_data(uint32_t index) const
    {
        assert(m_avcc != nullptr);
        return m_avcc->picture_parameter_set(index)->data();
    }

    uint32_t h264_sample_extractor::pps_size(uint32_t index) const
    {
        assert(m_avcc != nullptr);
        return m_avcc->picture_parameter_set(index)->size();
    }

    const uint8_t* h264_sample_extractor::sps_data(uint32_t index) const
    {
        assert(m_avcc != nullptr);
        return m_avcc->sequence_parameter_set(index)->data();
    }

    uint32_t h264_sample_extractor::sps_size(uint32_t index) const
    {
        assert(m_avcc != nullptr);
        return m_avcc->sequence_parameter_set(index)->size();
    }

    uint64_t h264_sample_extractor::decoding_timestamp() const
    {
        assert(m_stts != nullptr);
        return decoding_time(m_stts, m_timescale, m_sample);
    }

    uint64_t h264_sample_extractor::presentation_timestamp() const
    {
        assert(m_stts != nullptr);
        return presentation_time(m_stts, m_ctts, m_timescale, m_sample);
    }

    uint8_t h264_sample_extractor::nalu_length_size() const
    {
        assert(m_avcc != nullptr);
        return m_avcc->length_size();
    }
}
}
