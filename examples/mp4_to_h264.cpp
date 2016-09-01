// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <fstream>
#include <string>

#include <petro/extractor/h264_extractor.hpp>

int main(int argc, char* argv[])
{
    // if (argc != 3 || std::string(argv[1]) == "--help")
    // {
    //     auto usage = "./mp4_to_h264 MP4_INPUT H264_OUTPUT";
    //     std::cout << usage << std::endl;
    //     return 0;
    // }

    // auto filename = std::string(argv[1]);

    // std::ifstream check(filename, std::ios::binary);

    // if (!check.is_open() || !check.good())
    // {
    //     std::cerr << "Error reading file: " << filename << std::endl;
    //     return 1;
    // }

    // petro::extractor::h264_extractor extractor(filename);

    // // Create the h264 output file
    // std::ofstream h264_file(argv[2], std::ios::binary);

    // // Write the sps and pps first
    // h264_file.write((char*)extractor.sps().data(), extractor.sps().size());
    // h264_file.write((char*)extractor.pps().data(), extractor.pps().size());

    // // Write the h264 samples (a single sample might contain multiple NALUs)
    // while (extractor.load_next_sample())
    // {
    //     auto sample = extractor.sample_data();
    //     h264_file.write((char*)sample.data(), sample.size());
    // }

    // h264_file.close();

    return 0;
}
