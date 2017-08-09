// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/ipmc.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_ipmc, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'i', 'p', 'm', 'c'
    };
    auto ipmc_box = std::make_shared<petro::box::ipmc>(
        buffer.data(), buffer.size());

    std::error_code error;
    ipmc_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("ipmc", ipmc_box->type());
}
