// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stsh.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stsh, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            's', 't', 's', 'h'
        };
    auto stsh_box = std::make_shared<petro::box::stsh>(
        buffer.data(), buffer.size());

    std::error_code error;
    stsh_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stsh", stsh_box->type());
}
