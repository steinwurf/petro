// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <petro/extractor/h264_extractor.hpp>

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

    std::ifstream mp4_file(filename, std::ios::binary);
    auto extractor = petro::extractor::h264_extractor(mp4_file);

    // Create the h264 output file
    std::ofstream h264_file(argv[2], std::ios::binary);

    // Write the sps and pps first
    std::vector<char> start_code = {0, 0, 0, 1};
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.sps()->data(), extractor.sps()->size());
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.pps()->data(), extractor.pps()->size());

    // Write the video samples
    int nalu_number = 0;
    while (extractor.has_next_nalu())
    {
        nalu_number++;
        h264_file.write(start_code.data(), start_code.size());
        auto next_nalu = extractor.next_nalu();
        h264_file.write(next_nalu.data(), next_nalu.size());
    }

    std::cout << "Amount of nalus: " << nalu_number << std::endl;

    mp4_file.close();
    h264_file.close();
    return 0;
}
