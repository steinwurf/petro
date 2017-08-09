// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/mfhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_mfhd, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'm', 'f', 'h', 'd'
        };
    auto mfhd_box = std::make_shared<petro::box::mfhd>(
        buffer.data(), buffer.size());

    std::error_code error;
    mfhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("mfhd", mfhd_box->type());
}
