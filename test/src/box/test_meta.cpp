// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/meta.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_meta, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
         'm',  'e',  't',  'a',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto meta_box = std::make_shared<petro::box::meta<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    meta_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("meta", meta_box->type());
    EXPECT_EQ(buffer.size(), meta_box->size());
    EXPECT_EQ(1U, meta_box->children().size());
}
