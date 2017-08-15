// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stts.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stts, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            's', 't', 't', 's',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
    auto stts_box = std::make_shared<petro::box::stts>(
        buffer.data(), buffer.size());

    std::error_code error;
    stts_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stts", stts_box->type());
}
