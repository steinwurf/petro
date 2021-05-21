// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/smhd.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_smhd, construct)
{
    std::vector<uint8_t> buffer = {
        0x00, 0x00, 0x00, 0x00, 's',  'm',  'h',  'd',
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    auto smhd_box =
        std::make_shared<petro::box::smhd>(buffer.data(), buffer.size());

    std::error_code error;
    smhd_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("smhd", smhd_box->type());
}
