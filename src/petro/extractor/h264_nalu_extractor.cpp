// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "h264_nalu_extractor.hpp"

#include <cassert>
#include <cstdint>
#include <string>

namespace petro
{
namespace extractor
{
    bool h264_nalu_extractor::open(const std::string& filename)
    {
        // Try to open sample_extractor
        auto success = m_sample_extractor.open(filename);
        if (!success)
            return false;
        reset();
        return true;
    }

    void h264_nalu_extractor::close()
    {
        m_sample_extractor.close();
    }

    void h264_nalu_extractor::advance()
    {
        assert(!at_end());

        // increment the offset by the size of the previous nalu.
        m_sample_offset += m_nalu_size;

        // If we are at the end of this sample ...
        if (m_sample_offset == m_sample_extractor.sample_size())
        {
            // ... reset state
            m_sample_offset = 0;
            m_nalu_size = 0;

            // and get the next one
            m_sample_extractor.advance();

            // only read the nalu size of we are not at the end.
            if (at_end())
                return;
        }
        read_nalu_size();
    }

    bool h264_nalu_extractor::at_end() const
    {
        return m_sample_extractor.at_end() && m_sample_offset == 0;
    }

    void h264_nalu_extractor::reset()
    {
        m_sample_extractor.reset();
        m_sample_offset = 0;

        // only read the nalu size if there's actually any samples to read.
        if (!at_end())
            read_nalu_size();
    }

    const uint8_t* h264_nalu_extractor::nalu_data() const
    {
        assert(!at_end());
        return m_sample_offset + m_sample_extractor.sample_data();
    }

    uint32_t h264_nalu_extractor::nalu_size() const
    {
        assert(!at_end());
        return m_nalu_size;
    }

    void h264_nalu_extractor::read_nalu_size()
    {
        assert(!at_end());
        auto length_size = m_sample_extractor.nalu_length_size();
        auto data = m_sample_offset + m_sample_extractor.sample_data();
        m_sample_offset += length_size;


        m_nalu_size = 0;
        for (uint8_t i = 0; i < length_size; ++i)
        {
            m_nalu_size |= ((uint32_t)data[i]) << ((length_size - 1) - i) * 8;
        }
    }

    bool h264_nalu_extractor::is_new_sample() const
    {
        return m_sample_offset == m_sample_extractor.nalu_length_size();
    }

    uint64_t h264_nalu_extractor::timestamp() const
    {
        return m_sample_extractor.timestamp();
    }

    const uint8_t* h264_nalu_extractor::pps_data(uint32_t index) const
    {
        return m_sample_extractor.pps_data(index);
    }

    uint32_t h264_nalu_extractor::pps_size(uint32_t index) const
    {
        return m_sample_extractor.pps_size(index);
    }

    const uint8_t* h264_nalu_extractor::sps_data(uint32_t index) const
    {
        return m_sample_extractor.sps_data(index);
    }

    uint32_t h264_nalu_extractor::sps_size(uint32_t index) const
    {
        return m_sample_extractor.sps_size(index);
    }

    uint64_t h264_nalu_extractor::decoding_timestamp() const
    {
        return m_sample_extractor.decoding_timestamp();
    }

    uint64_t h264_nalu_extractor::presentation_timestamp() const
    {
        return m_sample_extractor.presentation_timestamp();
    }
}
}
