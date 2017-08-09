// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/trun.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_trun, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            't', 'r', 'u', 'n'
        };
    auto trun_box = std::make_shared<petro::box::trun>(
        buffer.data(), buffer.size());

    std::error_code error;
    trun_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("trun", trun_box->type());
}
