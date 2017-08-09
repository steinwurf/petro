// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/elst.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_elst, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'e', 'l', 's', 't',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
    auto elst_box = std::make_shared<petro::box::elst>(
        buffer.data(), buffer.size());

    std::error_code error;
    elst_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("elst", elst_box->type());
}
