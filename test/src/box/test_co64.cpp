// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/co64.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_co64, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'c', 'o', '6', '4',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
    auto co64_box = std::make_shared<petro::box::co64>(
        buffer.data(), buffer.size());

    std::error_code error;
    co64_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("co64", co64_box->type());
}
