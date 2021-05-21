// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/mfra.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_mfra, construct)
{
    std::vector<uint8_t> buffer = {
        0x00, 0x00, 0x00, 0x10, 'm',  'f',  'r',  'a',
        0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
    };
    auto mfra_box = std::make_shared<petro::box::mfra<petro::parser<>>>(
        buffer.data(), buffer.size());

    std::error_code error;
    mfra_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("mfra", mfra_box->type());
    EXPECT_EQ(buffer.size(), mfra_box->size());
    EXPECT_EQ(1U, mfra_box->children().size());
}
