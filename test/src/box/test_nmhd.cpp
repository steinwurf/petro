// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/nmhd.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_nmhd, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'n', 'm', 'h', 'd',
            0x00, 0x00, 0x00, 0x00
        };
    auto nmhd_box = std::make_shared<petro::box::nmhd>(
        buffer.data(), buffer.size());

    std::error_code error;
    nmhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("nmhd", nmhd_box->type());
}
