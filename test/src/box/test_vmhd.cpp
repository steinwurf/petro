// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/vmhd.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_vmhd, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'v', 'm', 'h', 'd',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };
    auto vmhd_box = std::make_shared<petro::box::vmhd>(
        buffer.data(), buffer.size());

    std::error_code error;
    vmhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("vmhd", vmhd_box->type());
}
