// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/traf.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_traf, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x10,
            't', 'r', 'a', 'f',
            0x00, 0x00, 0x00, 0x08,
            0x00, 0x00, 0x00, 0x00,
        };
    auto traf_box = std::make_shared<petro::box::traf<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    traf_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("traf", traf_box->type());
    EXPECT_EQ(buffer.size(), traf_box->size());
    EXPECT_EQ(1U, traf_box->children().size());
}
