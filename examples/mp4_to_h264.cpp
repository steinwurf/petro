// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/parser.hpp>
#include <petro/box/all.hpp>
#include <petro/extractor/h264_extractor.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3 || std::string(argv[1]) == "--help")
    {
        auto usage = "./mp4_to_h264 MP4_INPUT H264_OUTPUT";
        std::cout << usage << std::endl;
        return 0;
    }

    auto filename = std::string(argv[1]);


    std::ifstream check(filename, std::ios::binary);

    if (!check.is_open() || !check.good())
    {
        std::cerr << "Error reading file: " << filename << std::endl;
        return 1;
    }


    // write video data
    std::ifstream mp4_file(filename, std::ios::binary);

    // create output file
    std::ofstream h264_file(argv[2], std::ios::binary);

    auto extractor = petro::h264_extractor(mp4_file);


    std::vector<char> start_code = {0, 0, 0, 1};
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.sps()->data(), extractor.sps()->size());
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.pps()->data(), extractor.pps()->size());

    int nalu_number = 0;
    while (extractor.has_next_nalu())
    {
        nalu_number++;
        h264_file.write(start_code.data(), start_code.size());
        auto next_nalu = extractor.next_nalu();
        h264_file.write(next_nalu.data(), next_nalu.size());
    }

    mp4_file.close();
    h264_file.close();
    return 0;
}
