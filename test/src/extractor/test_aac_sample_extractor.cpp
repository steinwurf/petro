// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/aac_sample_extractor.hpp>

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
}

TEST(extractor_test_aac_sample_extractor, test_aac_file)
{
    auto test_filename = "test1.aac";
    std::ifstream test_aac(test_filename, std::ios::binary);
    EXPECT_TRUE(test_aac.is_open());
    EXPECT_TRUE(test_aac.good());

    petro::extractor::aac_sample_extractor extractor;
    extractor.set_file_path("test1.mp4");
    EXPECT_EQ("test1.mp4", extractor.file_path());
    EXPECT_TRUE(extractor.open());
    EXPECT_EQ(7U, extractor.adts_header_size());

    std::vector<uint8_t> adts_header(extractor.adts_header_size());

    EXPECT_FALSE(extractor.at_end());
    while (!extractor.at_end())
    {
        extractor.write_adts_header(adts_header.data());
        check_sample(test_aac, adts_header.data(), adts_header.size());

        auto sample_data = extractor.sample_data();
        auto sample_size = extractor.sample_size();

        check_sample(test_aac, sample_data, sample_size);

        extractor.advance();
    }
    test_aac.close();
}
