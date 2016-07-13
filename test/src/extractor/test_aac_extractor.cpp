// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/aac_extractor.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>

#include <gtest/gtest.h>

TEST(test_aac_extractor, test_aac_file)
{
    // Open file with sanitised test data
    // File generate with ffmpeg: ffmpeg -i test.mp4 -acodec copy test.aac
    auto test_filename = "test.aac";
    std::ifstream test_aac(test_filename, std::ios::binary);
    EXPECT_TRUE(test_aac.is_open());
    EXPECT_TRUE(test_aac.good());

    petro::extractor::aac_extractor aac_extractor("test.mp4");

    // Check that each sample is correct
    while (aac_extractor.advance_to_next_sample())
    {
        auto sample = aac_extractor.sample_data();
        std::vector<uint8_t> temp(sample.size());
        test_aac.read((char*)temp.data(), temp.size());
        EXPECT_EQ(sample, temp);
    }

    test_aac.close();
}
