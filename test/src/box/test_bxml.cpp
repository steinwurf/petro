// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/bxml.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_bxml, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'b', 'x', 'm', 'l'
    };
    auto bxml_box = std::make_shared<petro::box::bxml>(
        buffer.data(), buffer.size());

    std::error_code error;
    bxml_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("bxml", bxml_box->type());
}
