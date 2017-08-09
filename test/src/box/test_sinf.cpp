// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/sinf.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_sinf, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x10,
            's', 'i', 'n', 'f',
            0x00, 0x00, 0x00, 0x08,
            0x00, 0x00, 0x00, 0x00,
        };
    auto sinf_box = std::make_shared<petro::box::sinf<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    sinf_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("sinf", sinf_box->type());
    EXPECT_EQ(buffer.size(), sinf_box->size());
    EXPECT_EQ(1U, sinf_box->children().size());
}
