// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/trak.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_trak, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x10,
            't', 'r', 'a', 'k',
            0x00, 0x00, 0x00, 0x08,
            0x00, 0x00, 0x00, 0x00,
        };
    auto trak_box = std::make_shared<petro::box::trak<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    trak_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("trak", trak_box->type());
    EXPECT_EQ(buffer.size(), trak_box->size());
    EXPECT_EQ(1U, trak_box->children().size());
}
