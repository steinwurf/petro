// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/mdia.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_mdia, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
         'm',  'd',  'i',  'a',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto mdia_box = std::make_shared<petro::box::mdia<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    mdia_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("mdia", mdia_box->type());
    EXPECT_EQ(buffer.size(), mdia_box->size());
    EXPECT_EQ(1U, mdia_box->children().size());
}
