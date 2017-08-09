// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/hmhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_hmhd, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
         'h',  'm',  'h',  'd',
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    auto hmhd_box = std::make_shared<petro::box::hmhd>(
        buffer.data(), buffer.size());

    std::error_code error;
    hmhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("hmhd", hmhd_box->type());
}
