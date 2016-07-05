// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/aac_extractor.hpp>

#include <fstream>
#include <string>
#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3 || std::string(argv[1]) == "--help")
    {
        auto usage = "./mp4_to_aac MP4_INPUT AAC_OUTPUT";
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
    // create output file
    std::ofstream aac_file(argv[2], std::ios::binary);

    // fill output file with data.
    std::ifstream mp4_file(filename, std::ios::binary);

    petro::extractor::aac_extractor extractor(mp4_file);

    while(extractor.has_next_adts())
    {
        auto next_adts = extractor.next_adts();
        aac_file.write(next_adts.data(), next_adts.size());
    }

    mp4_file.close();
    aac_file.close();

    return 0;
}
