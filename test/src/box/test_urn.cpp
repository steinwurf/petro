// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/urn.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_urn, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'u', 'r', 'n', ' '
    };
    auto urn_box = std::make_shared<petro::box::urn>(
        buffer.data(), buffer.size());

    std::error_code error;
    urn_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("urn ", urn_box->type());
}
