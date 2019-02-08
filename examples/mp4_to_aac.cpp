// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <petro/extractor/aac_sample_extractor.hpp>
#include <petro/extractor/file.hpp>

int main(int argc, char* argv[])
{
    if (argc != 4 || std::string(argv[1]) == "--help")
    {
        auto usage = "./mp4_to_aac MP4_INPUT TRACK_ID AAC_OUTPUT";
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
    petro::extractor::aac_sample_extractor extractor;
    uint32_t track_id = strtol(argv[2], NULL, 10);
    extractor.open(file.data(), file.size(), track_id, error);
    if (error)
    {
        std::cerr << "Error. Unable to extract aac from: "
                  << filename << std::endl
                  << "Error message: " << error.message() << std::endl;
        return 1;
    }

    // Create the aac output file
    std::ofstream aac_file(argv[3], std::ios::binary);

    std::vector<uint8_t> adts_header(extractor.adts_header_size());
    // Write the adts samples
    while (!extractor.at_end())
    {
        extractor.write_adts_header(adts_header.data());
        aac_file.write((char*)adts_header.data(), adts_header.size());
        aac_file.write((char*)extractor.sample_data(), extractor.sample_size());
        extractor.advance();
    }

    aac_file.close();
    file.close();

    return 0;
}
