// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/nalu_extractor.hpp>

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

    void test_h264_file(
        const std::string& h264_file,
        const std::vector<bool>& expected_beginning_of_avc_samples,
        const std::string& mp4_file)
    {
        std::ifstream test_h264(h264_file, std::ios::binary);
        EXPECT_TRUE(test_h264.is_open());
        EXPECT_TRUE(test_h264.good());

        petro::extractor::nalu_extractor extractor;
        extractor.set_file_path(mp4_file);
        EXPECT_EQ(mp4_file, extractor.file_path());
        EXPECT_TRUE(extractor.open());

        std::vector<uint8_t> nalu_header(extractor.nalu_header_size());
        extractor.write_nalu_header(nalu_header.data());

        check_sample(test_h264, nalu_header.data(), nalu_header.size());
        check_sample(test_h264, extractor.sps_data(), extractor.sps_size());

        check_sample(test_h264, nalu_header.data(), nalu_header.size());
        check_sample(test_h264, extractor.pps_data(), extractor.pps_size());

        uint32_t i = 0;
        while (!extractor.at_end())
        {
            EXPECT_EQ(
                expected_beginning_of_avc_samples[i],
                extractor.is_beginning_of_avc_sample());

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
}

TEST(extractor_test_nalu_extractor, test1_h264_file)
{
    // this data has been collected from the test1.mp4 file. It isn't verified to
    // be correct but it's used for testing consistency.
    std::vector<bool> expected_beginning_of_avc_samples =
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

    test_h264_file("test1.h264", expected_beginning_of_avc_samples, "test1.mp4");
}

TEST(extractor_test_nalu_extractor, test2_h264_file)
{
    std::vector<bool> expected_beginning_of_avc_samples;

    test_h264_file("test2.h264", expected_beginning_of_avc_samples, "test2.mp4");
}

TEST(extractor_test_nalu_extractor, test3_h264_file)
{
    // this data has been collected from the test1.mp4 file. It isn't verified to
    // be correct but it's used for testing consistency.
    std::vector<bool> expected_beginning_of_avc_samples =
    {
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1
    };

    test_h264_file("test3.h264", expected_beginning_of_avc_samples, "test3.mp4");
}
