// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <petro/extractor/track_type_to_string.hpp>
#include <petro/extractor/track_extractor.hpp>
#include <petro/extractor/avc_to_annex_b.hpp>
#include <petro/extractor/avc_sample_extractor.hpp>
#include <petro/extractor/file.hpp>

int main(int argc, char* argv[])
{
    if (argc != 4 || std::string(argv[1]) == "--help")
    {
        auto usage = "./mp4_to_h264 TRACK_ID MP4_INPUT H264_OUTPUT";
        std::cout << usage << std::endl;
        return 0;
    }

    auto filename = std::string(argv[1]);

    petro::extractor::file file;
    std::error_code error;
    file.open(filename, error);
    if (error)
    {
        std::cerr << "File not found: " << filename << std::endl;
        return 1;
    }

    petro::extractor::avc_sample_extractor extractor;
    uint32_t track_id = strtol(argv[2], NULL, 10);
    extractor.open(file.data(), file.size(), track_id, error);
    if (error)
    {
        std::cerr << "Error. Unable to extract h264 from: "
                  << filename << std::endl
                  << "Error message: " << error.message() << std::endl;
        std::error_code error;
        petro::extractor::track_extractor track_extractor;
        track_extractor.open(file.data(), file.size(), error);
        std::cout << "Available tracks:" << '\n';
        for (auto track : track_extractor.tracks())
        {
            std::cout << "- Track " << track.id << ": " << petro::extractor::track_type_to_string(track.type) << std::endl;
        }
        return 1;
    }

    // Create the h264 output file
    std::ofstream h264_file(argv[3], std::ios::binary);

    // Create start code buffer
    std::vector<char> start_code(extractor.nalu_header_size());
    extractor.write_nalu_header((uint8_t*)start_code.data());

    // Write the sps and pps first
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.sps_data(), extractor.sps_size());
    h264_file.write(start_code.data(), start_code.size());
    h264_file.write((char*)extractor.pps_data(), extractor.pps_size());

    // Write the h264 samples (a single sample might contain multiple NALUs)
    while (!extractor.at_end())
    {
        auto nalus = petro::extractor::avc_to_annex_b(
            extractor.sample_data(),
            extractor.sample_size(),
            extractor.nalu_length_size());
        for (auto nalu : nalus)
        {
            h264_file.write(start_code.data(), start_code.size());
            auto data = nalu.first;
            auto size = nalu.second;
            h264_file.write((char*)data, size);
        }
        extractor.advance();
    }

    h264_file.close();
    file.close();

    return 0;
}
