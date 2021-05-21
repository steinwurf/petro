// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/stbl.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stbl, construct)
{
    std::vector<uint8_t> buffer = {
        0x00, 0x00, 0x00, 0x10, 's',  't',  'b',  'l',
        0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
    };
    auto stbl_box = std::make_shared<petro::box::stbl<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    stbl_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stbl", stbl_box->type());
    EXPECT_EQ(buffer.size(), stbl_box->size());
    EXPECT_EQ(1U, stbl_box->children().size());
}
