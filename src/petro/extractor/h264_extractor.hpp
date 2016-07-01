// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../parser.hpp"
#include "../box/all.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <memory>

namespace petro
{
namespace extractor
{
    class h264_extractor
    {
    public:
        h264_extractor(std::ifstream& file);
        //~h264_extractor();

        const std::shared_ptr<sequence_parameter_set> sps();
        const std::shared_ptr<picture_parameter_set> pps();
        uint32_t found_samples();
        std::vector<char> next_nalu();
        uint32_t current_nalu_size();
        bool has_next_nalu();


    private:
        uint32_t read_nalu_size(std::istream& file, uint8_t length_size);

    private:
        std::ifstream& m_file;
        uint32_t m_found_samples = 0;
        uint32_t m_sample = 0;
        uint32_t m_sample_size;
        uint32_t m_chunk = 0;
        uint32_t m_current_nalu_size = 0;
        std::vector<uint64_t> m_chunk_offsets;
        std::shared_ptr<const box::avcc> m_avcc;
        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;
    };
}
}
