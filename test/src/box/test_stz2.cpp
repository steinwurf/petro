// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stz2.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stz2, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            's', 't', 'z', '2',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
            0x00,
            0x00, 0x00, 0x00, 0x00
        };
    auto stz2_box = std::make_shared<petro::box::stz2>(
        buffer.data(), buffer.size());

    std::error_code error;
    stz2_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stz2", stz2_box->type());
}
