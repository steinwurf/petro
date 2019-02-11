// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/avc_to_annex_b.hpp>

#include <cstdint>
#include <vector>
#include <numeric>
#include <fstream>

#include <gtest/gtest.h>

TEST(extractor_test_avc_to_annex_b, init)
{
    auto mp4_file = "test1.mp4";
    std::ifstream test_mp4(mp4_file, std::ios::binary);
    EXPECT_TRUE(test_mp4.is_open());
    EXPECT_TRUE(test_mp4.good());

    auto sample_offset = 218U;
    auto sample_size = 28521U;
    auto nalu_size_length = 4U;

    // skip nalu start codes
    test_mp4.seekg(sample_offset, std::ios::cur);

    // read data
    std::vector<uint8_t> test_sample(sample_size);
    test_mp4.read((char*)test_sample.data(), test_sample.size());

    auto samples = petro::extractor::avc_to_annex_b(
        test_sample.data(), test_sample.size(), nalu_size_length);

    EXPECT_EQ(5U, samples.size());
    auto sum_size = std::accumulate(samples.begin(), samples.end(), 0U,
        [](uint64_t accumlate, auto s){ return accumlate + s.second; });
    auto expected_sum_size = sample_size - (samples.size() * nalu_size_length);
    EXPECT_EQ(expected_sum_size, sum_size);
    test_mp4.close();
}
