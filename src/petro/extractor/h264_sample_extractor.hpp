// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "../box/avcc.hpp"
#include "../box/ctts.hpp"
#include "../box/stsc.hpp"
#include "../box/stsz.hpp"
#include "../box/stts.hpp"

#include <boost/iostreams/device/mapped_file.hpp>

namespace petro
{
namespace extractor
{
    class h264_sample_extractor
    {
    public:

        h264_sample_extractor(bool loop);

        bool open(const std::string& filename);

        bool at_end() const;

        void advance();

        const uint8_t* sample_data() const;
        uint32_t sample_size() const;

        const uint8_t* pps_data(uint32_t index) const;
        uint32_t pps_size(uint32_t index) const;

        const uint8_t* sps_data(uint32_t index) const;
        uint32_t sps_size(uint32_t index) const;

        uint64_t decoding_timestamp() const;

        uint64_t presentation_timestamp() const;

        uint8_t nalu_length_size() const;
    private:


        std::string m_filename;
        bool m_loop;
        boost::iostreams::mapped_file_source m_file;

        uint32_t m_sample = 0;
        uint32_t m_chunk_index = 0;
        uint32_t m_chunk_sample = 0;
        uint32_t m_loop_offset = 0;
        uint32_t m_timescale = 0;

        std::vector<uint64_t> m_chunk_offsets;

        std::shared_ptr<const box::avcc> m_avcc;
        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;
        std::shared_ptr<const box::stts> m_stts;
        std::shared_ptr<const box::ctts> m_ctts;
    };
}
}
