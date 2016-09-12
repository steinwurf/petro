// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/h264_nalu_extractor.hpp>

#include <cstdint>
#include <vector>
#include <fstream>
#include <string>

#include <gtest/gtest.h>

namespace
{
void check_sample(std::ifstream& expected, const uint8_t* data, uint32_t size)
{
    // read data
    std::vector<uint8_t> expected_sample(size);
    expected.read((char*)expected_sample.data(), expected_sample.size());
    std::vector<uint8_t> actual_sample(data, data + size);

    ASSERT_EQ(expected_sample, actual_sample);
}
}

TEST(extractor_test_h264_nalu_extractor, test_h264_file)
{
    auto test_filename = "test.h264";
    std::ifstream test_h264(test_filename, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());
    EXPECT_TRUE(test_h264.good());

    petro::extractor::h264_nalu_extractor extractor;
    extractor.set_file_path("test.mp4");
    EXPECT_EQ("test.mp4", extractor.file_path());
    EXPECT_TRUE(extractor.open());

    std::vector<uint8_t> nalu_header(extractor.nalu_header_size());
    extractor.write_nalu_header(nalu_header.data());

    check_sample(test_h264, nalu_header.data(), nalu_header.size());
    check_sample(test_h264, extractor.sps_data(), extractor.sps_size());

    check_sample(test_h264, nalu_header.data(), nalu_header.size());
    check_sample(test_h264, extractor.pps_data(), extractor.pps_size());

    EXPECT_FALSE(extractor.at_end());

    std::vector<bool> expected_new_sample =
    {
        1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0
    };

    uint32_t i = 0;
    while (!extractor.at_end())
    {
        EXPECT_EQ(expected_new_sample[i], extractor.is_new_sample());

        check_sample(test_h264, nalu_header.data(), nalu_header.size());

        auto nalu_data = extractor.nalu_data();
        auto nalu_size = extractor.nalu_size();
        check_sample(test_h264, nalu_data, nalu_size);

        extractor.advance();
        ++i;
    }

    // expect test_h264 to be eof.
    EXPECT_EQ(std::istream::traits_type::eof(), test_h264.get());
    test_h264.close();
}
