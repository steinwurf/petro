// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/picture_parameter_set.hpp>

#include <vector>

#include <gtest/gtest.h>

TEST(test_picture_parameter_set, init)
{
    std::vector<uint8_t> buffer = {0x68, 0xce, 0x38, 0x80};

    std::error_code error;
    auto pps = petro::picture_parameter_set::parse(buffer.data(), buffer.size(),
                                                   error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(buffer.size(), pps->size());
    EXPECT_EQ(buffer.data(), pps->data());
}
