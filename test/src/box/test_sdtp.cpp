// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/sdtp.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_sdtp, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        's', 'd', 't', 'p'
    };
    auto sdtp_box = std::make_shared<petro::box::sdtp>(
        buffer.data(), buffer.size());

    std::error_code error;
    sdtp_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("sdtp", sdtp_box->type());
}
