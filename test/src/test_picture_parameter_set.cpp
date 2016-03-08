// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/picture_parameter_set.hpp>

#include <vector>

#include <gtest/gtest.h>

TEST(test_picture_parameter_set, init)
{
    std::vector<uint8_t> data = {0x68, 0xce, 0x38, 0x80};

    petro::picture_parameter_set pps(data);
    EXPECT_EQ(data.size(), pps.size());
}
