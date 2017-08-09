// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/edts.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_edts, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x10,
        'e', 'd', 't', 's',
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00, 0x00, 0x00,
    };
    auto edts_box = std::make_shared<petro::box::edts<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    edts_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("edts", edts_box->type());
    EXPECT_EQ(buffer.size(), edts_box->size());
    EXPECT_EQ(1U, edts_box->children().size());
}
