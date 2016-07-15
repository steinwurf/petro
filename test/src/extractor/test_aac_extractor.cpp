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

    petro::extractor::aac_extractor extractor("test.mp4");

    // Check the audio properties of the test video
    EXPECT_EQ(3U, extractor.frequency_index());
    EXPECT_EQ(2U, extractor.channel_configuration());
    EXPECT_EQ(2U, extractor.mpeg_audio_object_type());

    uint32_t sample_number = 0;
    uint64_t last_timestamp = 0;
    while (extractor.load_next_sample())
    {
        // Check that each sample is correct
        auto sample = extractor.sample_data();
        std::vector<uint8_t> temp(sample.size());
        test_aac.read((char*)temp.data(), temp.size());
        EXPECT_EQ(sample, temp);

        // Check that the decoding timestamps are monotonically increasing
        uint64_t timestamp = extractor.decoding_timestamp();
        if (sample_number > 0)
            EXPECT_GT(timestamp, last_timestamp);
        EXPECT_EQ(last_timestamp + extractor.sample_delta(), timestamp);
        last_timestamp = timestamp;
        sample_number++;
    }
    EXPECT_EQ(true, extractor.at_end());

    test_aac.close();
}
