// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/avcc.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_avcc, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'a', 'v', 'c', 'C',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };
    auto avcc_box = std::make_shared<petro::box::avcc>(
        buffer.data(), buffer.size());

    std::error_code error;
    avcc_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("avcC", avcc_box->type());
}
