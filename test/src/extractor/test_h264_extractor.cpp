// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/h264_extractor.hpp>

#include <cstdint>
#include <vector>
#include <fstream>

#include <gtest/gtest.h>

TEST(test_h264_extractor, test_h264_file)
{
    auto test_filename = "test.h264";
    std::ifstream test_h264(test_filename, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());
    EXPECT_TRUE(test_h264.good());

    petro::extractor::h264_extractor extractor("test.mp4");

    // Get the SPS data from the extractor
    std::vector<uint8_t> extracted_sps = extractor.sps();
    std::vector<uint8_t> correct_sps(extracted_sps.size());
    test_h264.read((char*)correct_sps.data(), correct_sps.size());
    EXPECT_EQ(correct_sps, extracted_sps);

    // Get the PPS data from the extractor
    std::vector<uint8_t> extracted_pps = extractor.pps();
    std::vector<uint8_t> correct_pps(extracted_pps.size());
    test_h264.read((char*)correct_pps.data(), correct_pps.size());
    EXPECT_EQ(correct_pps, extracted_pps);

    while (extractor.advance_to_next_sample())
    {
        auto sample = extractor.sample_data();
        std::vector<uint8_t> temp(sample.size());
        test_h264.read((char*)temp.data(), temp.size());
        EXPECT_EQ(temp, sample);
    }

    test_h264.close();
}
