// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stco.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stco, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
         's',  't',  'c',  'o',
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    auto stco_box = std::make_shared<petro::box::stco>(
        buffer.data(), buffer.size());

    std::error_code error;
    stco_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stco", stco_box->type());
}
