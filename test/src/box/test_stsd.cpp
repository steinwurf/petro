// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stsd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stsd, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        's', 't', 's', 'd',
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    auto stsd_box = std::make_shared<petro::box::stsd>(
        buffer.data(), buffer.size());

    std::error_code error;
    stsd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stsd", stsd_box->type());
}
