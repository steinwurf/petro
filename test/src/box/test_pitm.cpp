// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/pitm.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_pitm, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'p', 'i', 't', 'm'
    };
    auto pitm_box = std::make_shared<petro::box::pitm>(
        buffer.data(), buffer.size());

    std::error_code error;
    pitm_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("pitm", pitm_box->type());
}
