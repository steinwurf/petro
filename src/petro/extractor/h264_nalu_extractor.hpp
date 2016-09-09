// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <cstdint>

#include "h264_sample_extractor.hpp"

namespace petro
{
namespace extractor
{
    class h264_nalu_extractor
    {
    public:

        bool open(const std::string& filename);

        void close();

        void advance();

        bool at_end() const;

        void reset();

        const uint8_t* nalu_data() const;

        uint32_t nalu_size() const;

        bool is_new_sample() const;

    private:

        void read_nalu_size();

    public:

        // Calls just forwarding to the h264_sample_extractor

        uint64_t timestamp() const;

        const uint8_t* pps_data(uint32_t index) const;
        uint32_t pps_size(uint32_t index) const;

        const uint8_t* sps_data(uint32_t index) const;
        uint32_t sps_size(uint32_t index) const;

        uint64_t decoding_timestamp() const;

        uint64_t presentation_timestamp() const;

    private:

        uint32_t m_sample_offset = 0;
        uint32_t m_nalu_size = 0;

        h264_sample_extractor m_sample_extractor;
    };
}
}
