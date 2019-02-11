// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/avc_sample_extractor.hpp>
#include <petro/extractor/file.hpp>

#include <cstdint>
#include <vector>
#include <fstream>

#include <gtest/gtest.h>

namespace
{
void check_sample(std::ifstream& expected, const uint8_t* data, uint32_t size)
{
    // skip nalu start codes
    expected.seekg(4, std::ios::cur);

    // read data
    std::vector<uint8_t> expected_sample(size);
    expected.read((char*)expected_sample.data(), expected_sample.size());

    std::vector<uint8_t> actual_sample(data, data + size);

    ASSERT_EQ(expected_sample, actual_sample);
}

uint32_t read_nalu_size(const uint8_t* data, uint8_t length_size)
{
    uint32_t result = 0;
    for (uint8_t i = 0; i < length_size; ++i)
    {
        result |= data[i] << ((length_size - 1) - i) * 8;
    }
    return result;
}

void test_h264_file(
    const std::string& h264_file,
    const std::string& mp4_file,
    uint32_t expected_samples)
{
    std::ifstream test_h264(h264_file, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());
    EXPECT_TRUE(test_h264.good());

    std::error_code error;
    petro::extractor::file file;
    file.open(mp4_file, error);
    ASSERT_FALSE(bool(error));

    petro::extractor::avc_sample_extractor extractor;
    auto track_id = 1; // The track id of the test file's h264 trak is 2
    extractor.open(file.data(), file.size(), track_id, error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ(expected_samples, extractor.samples());

    check_sample(test_h264, extractor.sps_data(), extractor.sps_size());
    check_sample(test_h264, extractor.pps_data(), extractor.pps_size());

    auto nalu_length_size = extractor.nalu_length_size();

    while (!extractor.at_end())
    {
        auto sample_data = extractor.sample_data();
        auto sample_size = extractor.sample_size();

        while ((uint32_t)(sample_data - extractor.sample_data()) < sample_size)
        {
            uint32_t nalu_size = read_nalu_size(sample_data, nalu_length_size);
            sample_data += nalu_length_size;

            check_sample(test_h264, sample_data, nalu_size);
            sample_data += nalu_size;
        }

        extractor.advance();
    }
    test_h264.close();
}
}

TEST(extractor_test_avc_sample_extractor, test1_h264_file)
{
    // ~ One sample per chunk, multiple nalus per sample
    auto samples = 166U;
    test_h264_file("test1.h264", "test1.mp4", samples);
}

TEST(extractor_test_avc_sample_extractor, test2_h264_file)
{
    // No samples
    auto samples = 0U;
    test_h264_file("test2.h264", "test2.mp4", samples);
}

TEST(extractor_test_avc_sample_extractor, test3_h264_file)
{
    // ~ Multiple sample per chunk, single nalu per sample
    auto samples = 148U;
    test_h264_file("test3.h264", "test3.mp4", samples);
}
