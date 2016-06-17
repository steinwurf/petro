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
    class h264_extractor
    {
    public:
        h264_extractor(std::ifstream& file);
        ~h264_extractor();

        std::shared_ptr<sequence_parameter_set> sps();
        std::shared_ptr<picture_parameter_set> pps();
        uint32_t found_samples();
        std::vector<char> next_nalu();
        bool has_next_nalu();


    private:
        uint32_t read_nalu_size();
        void setup();

    private:
        std::ifstream& m_file;
        std::string m_filename;
        uint32_t m_found_samples = 0;
        uint32_t m_sample_size;
        uint32_t m_chunk = 0;
        std::shared_ptr<const box::box> m_avc1;
        std::shared_ptr<const box::avcc> m_avcc;
        std::shared_ptr<box::box> m_trak;
        std::shared_ptr<const box::stco> m_stco;
        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;

        std::vector<uint64_t> m_chunk_offsets;

        parser<
            box::moov<parser<
            box::trak<parser<
            box::mdia<parser<
            box::hdlr,
                          box::minf<parser<
                                        box::stbl<
                                            parser<
                                                box::stsd,
                                                box::stsc,
                                                box::stco,
                                                box::co64,
                                                box::stsz
                                                >>
                                        >>
                          >>
                          >>
                          >>
            > m_parser;
    }
}
