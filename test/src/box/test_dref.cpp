// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/dref.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_dref, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x18,
        'd', 'r', 'e', 'f',
        0x00,
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };

    auto dref_box = std::make_shared<petro::box::dref<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    dref_box->parse(error);
    assert(!error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("dref", dref_box->type());
    EXPECT_EQ(buffer.size(), dref_box->size());
    EXPECT_EQ(1U, dref_box->children().size());
}
