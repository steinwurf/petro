// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/mvex.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_mvex, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
         'm',  'v',  'e',  'x',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto mvex_box = std::make_shared<petro::box::mvex<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    mvex_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("mvex", mvex_box->type());
    EXPECT_EQ(buffer.size(), mvex_box->size());
    EXPECT_EQ(1U, mvex_box->children().size());
}
