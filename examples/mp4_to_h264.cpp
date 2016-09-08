// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <fstream>
#include <string>

#include <petro/extractor/h264_nalu_extractor.hpp>

int main(int argc, char* argv[])
{
    if (argc != 3 || std::string(argv[1]) == "--help")
    {
        auto usage = "./mp4_to_h264 MP4_INPUT H264_OUTPUT";
        std::cout << usage << std::endl;
        return 0;
    }

    auto filename = std::string(argv[1]);

    petro::extractor::h264_nalu_extractor extractor;

    if (!extractor.open(filename))
    {
        std::cerr << "Error reading file: " << filename << std::endl;
        return 1;
    }

    // Create the h264 output file
    std::ofstream h264_file(argv[2], std::ios::binary);

    // Create start code buffer
    std::vector<char> start_code = { 0, 0, 0, 1 };

    // Write the sps and pps first
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.sps_data(0), extractor.sps_size(0));
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.pps_data(0), extractor.pps_size(0));

    // Write the h264 samples (a single sample might contain multiple NALUs)
    while (!extractor.at_end())
    {
        h264_file.write(start_code.data(), start_code.size());
        h264_file.write((char*)extractor.nalu_data(), extractor.nalu_size());
        extractor.advance();
    }

    h264_file.close();

    return 0;
}
