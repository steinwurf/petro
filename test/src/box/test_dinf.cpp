// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/dinf.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_dinf, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
        'd', 'i', 'n', 'f',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto dinf_box = std::make_shared<petro::box::dinf<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    dinf_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("dinf", dinf_box->type());
    EXPECT_EQ(buffer.size(), dinf_box->size());
    EXPECT_EQ(1U, dinf_box->children().size());
}
