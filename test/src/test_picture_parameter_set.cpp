// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/picture_parameter_set.hpp>

#include <vector>

#include <gtest/gtest.h>

TEST(test_picture_parameter_set, init)
{
    std::vector<uint8_t> data = {0x68, 0xce, 0x38, 0x80};

    petro::picture_parameter_set pps(data.data(), data.size());
    std::error_code error;
    pps.parse(error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(data.size(), pps.size());
    EXPECT_EQ(data.data(), pps.data());
}
