// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/moof.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_moof, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x10,
            'm', 'o', 'o', 'f',
            0x00, 0x00, 0x00, 0x08,
            0x00, 0x00, 0x00, 0x00,
        };
    auto moof_box = std::make_shared<petro::box::moof<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    moof_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("moof", moof_box->type());
    EXPECT_EQ(buffer.size(), moof_box->size());
    EXPECT_EQ(1U, moof_box->children().size());
}
