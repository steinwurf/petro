// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/annex_b_writer.hpp>

#include <cstdint>
#include <vector>
#include <fstream>

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

void test_h264_file(const std::string& h264_file, const std::string& mp4_file)
{
    std::ifstream test_h264(h264_file, std::ios::binary);
    EXPECT_TRUE(test_h264.is_open());
    EXPECT_TRUE(test_h264.good());

    petro::extractor::annex_b_writer extractor;
    extractor.set_file_path(mp4_file);
    EXPECT_EQ(mp4_file, extractor.file_path());
    EXPECT_TRUE(extractor.open());

    // skip nalu start codes
    test_h264.seekg(4, std::ios::cur);
    check_sample(test_h264, extractor.sps_data(), extractor.sps_size());
    // skip nalu start codes
    test_h264.seekg(4, std::ios::cur);
    check_sample(test_h264, extractor.pps_data(), extractor.pps_size());

    std::vector<uint8_t> buffer;
    while (!extractor.at_end())
    {
        buffer.resize(extractor.annex_b_size());
        extractor.write_annex_b(buffer.data());
        check_sample(test_h264, buffer.data(), buffer.size());
        extractor.advance();
    }
    test_h264.close();
}
}

TEST(extractor_test_annex_b_writer, test1_h264_file)
{
    // ~ One sample per chunk, multiple nalus per sample
    test_h264_file("test1.h264", "test1.mp4");
}

TEST(extractor_test_annex_b_writer, test2_h264_file)
{
    // No samples
    test_h264_file("test2.h264", "test2.mp4");
}

TEST(extractor_test_annex_b_writer, test3_h264_file)
{
    // ~ Multiple sample per chunk, single nalu per sample
    test_h264_file("test3.h264", "test3.mp4");
}
