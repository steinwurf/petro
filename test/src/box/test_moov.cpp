// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/moov.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_moov, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
        'm', 'o', 'o', 'v',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto moov_box = std::make_shared<petro::box::moov<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    moov_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("moov", moov_box->type());
    EXPECT_EQ(buffer.size(), moov_box->size());
    EXPECT_EQ(1U, moov_box->children().size());
}
