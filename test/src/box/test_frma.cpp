// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/frma.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_frma, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'f', 'r', 'm', 'a'
    };
    auto frma_box = std::make_shared<petro::box::frma>(
        buffer.data(), buffer.size());

    std::error_code error;
    frma_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("frma", frma_box->type());
}
