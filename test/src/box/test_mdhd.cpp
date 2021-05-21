// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/mdhd.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_mdhd, construct)
{
    std::vector<uint8_t> buffer = {
        0x00, 0x00, 0x00, 0x00, 'm',  'd',  'h',  'd',  0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto mdhd_box =
        std::make_shared<petro::box::mdhd>(buffer.data(), buffer.size());

    std::error_code error;
    mdhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("mdhd", mdhd_box->type());
}
