// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/ipro.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_ipro, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
        'i', 'p', 'r', 'o',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto ipro_box = std::make_shared<petro::box::ipro<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    ipro_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("ipro", ipro_box->type());
    EXPECT_EQ(buffer.size(), ipro_box->size());
    EXPECT_EQ(1U, ipro_box->children().size());
}
