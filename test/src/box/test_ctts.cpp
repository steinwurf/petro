// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/ctts.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_ctts, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
         'c',  't',  't',  's',
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };
    auto ctts_box = std::make_shared<petro::box::ctts>(
        buffer.data(), buffer.size());

    std::error_code error;
    ctts_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("ctts", ctts_box->type());
}
