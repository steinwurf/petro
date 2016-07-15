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

    // Check the dimensions of the test video
    EXPECT_EQ(560U, extractor.video_width());
    EXPECT_EQ(320U, extractor.video_height());

    uint32_t sample_number = 0;
    uint64_t last_timestamp = 0;
    while (extractor.load_next_sample())
    {
        // Check that each sample is correct
        auto sample = extractor.sample_data();
        std::vector<uint8_t> temp(sample.size());
        test_h264.read((char*)temp.data(), temp.size());
        EXPECT_EQ(temp, sample);

        // Check that the decoding timestamps are monotonically increasing
        uint64_t timestamp = extractor.decoding_timestamp();
        if (sample_number > 0)
            EXPECT_GT(timestamp, last_timestamp);
        EXPECT_EQ(last_timestamp + extractor.sample_delta(), timestamp);
        last_timestamp = timestamp;
        sample_number++;
    }
    EXPECT_EQ(true, extractor.at_end());

    test_h264.close();
}
