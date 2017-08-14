// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <string>

#include "../box/stco.hpp"
#include "../box/stsc.hpp"
#include "../box/stsz.hpp"
#include "../box/co64.hpp"
#include "../box/box.hpp"

namespace petro
{
namespace extractor
{
/// Uses the aac and avc track layers to extract samples.
template<class Super>
class sample_extractor_layer : public Super
{
public:

    /// Open this and the underlying layer
    void open(std::error_code& error)
    {
        assert(!error);
        Super::open(error);
        if (error)
        {
            Super::close();
            return;
        }
        auto trak = Super::trak();

        std::vector<uint64_t> chunk_offsets;

        auto stco = trak->template get_child<box::stco>();
        if (stco != nullptr)
        {
            chunk_offsets = stco->entries();
        }
        else
        {
            auto co64 = trak->template get_child<box::co64>();
            if (co64 == nullptr)
            {
                close();
                error = petro::error::co64_box_missing;
                return;
            }

            chunk_offsets = co64->entries();
        }

        auto stsc = trak->template get_child<box::stsc>();
        if (stsc == nullptr)
        {
            close();
            error = petro::error::stsc_box_missing;
            return;
        }

        auto stsz = trak->template get_child<box::stsz>();
        if (stsz == nullptr)
        {
            close();
            error = petro::error::stsz_box_missing;
            return;
        }

        reset();

        m_chunk_offsets = chunk_offsets;

        m_stsc = stsc;
        m_stsz = stsz;
    }

    /// Close this and the underlying layer
    void close()
    {
        m_chunk_offsets.clear();
        m_stsc.reset();
        m_stsz.reset();

        Super::close();
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
        assert(!at_end());
        m_offset += sample_size();
        m_sample_index += 1;
        m_chunk_sample += 1;
        if (m_chunk_sample >= m_stsc->samples_for_chunk(m_chunk_index))
        {
            m_chunk_index += 1;
            m_chunk_sample = 0;
            m_offset = 0;
        }
    }

    /// Return true if no more sample are available.
    bool at_end() const
    {
        assert(m_stsz != nullptr);
        return (m_sample_index >= m_stsz->sample_count());
    }

    /// Return a pointer to the sample data
    const uint8_t* sample_data() const
    {
        assert(!at_end());
        return Super::data() + m_chunk_offsets[m_chunk_index] + m_offset;
    }

    /// Return the size of the sample data
    uint32_t sample_size() const
    {
        assert(!at_end());
        assert(m_stsz != nullptr);
        return m_stsz->sample_size(m_sample_index);
    }

    /// Return the current sample index
    uint32_t sample_index() const
    {
        assert(!at_end());
        return m_sample_index;
    }

private:

    uint32_t m_offset = 0;
    uint32_t m_sample_index = 0;
    uint32_t m_chunk_index = 0;
    uint32_t m_chunk_sample = 0;

    std::vector<uint64_t> m_chunk_offsets;

    std::shared_ptr<const box::stsc> m_stsc;
    std::shared_ptr<const box::stsz> m_stsz;

};
}
}
