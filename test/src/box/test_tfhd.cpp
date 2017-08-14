// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/tfhd.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_tfhd, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            't', 'f', 'h', 'd'
        };
    auto tfhd_box = std::make_shared<petro::box::tfhd>(
        buffer.data(), buffer.size());

    std::error_code error;
    tfhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("tfhd", tfhd_box->type());
}
