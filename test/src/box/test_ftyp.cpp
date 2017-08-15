// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/ftyp.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_ftyp, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'f', 't', 'y', 'p',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
    auto ftyp_box = std::make_shared<petro::box::ftyp>(
        buffer.data(), buffer.size());

    std::error_code error;
    ftyp_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("ftyp", ftyp_box->type());
}
