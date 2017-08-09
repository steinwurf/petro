// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stdp.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stdp, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        's', 't', 'd', 'p'
    };
    auto stdp_box = std::make_shared<petro::box::stdp>(
        buffer.data(), buffer.size());

    std::error_code error;
    stdp_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stdp", stdp_box->type());
}
