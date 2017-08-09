// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/imif.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_imif, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'i', 'm', 'i', 'f'
    };
    auto imif_box = std::make_shared<petro::box::imif>(
        buffer.data(), buffer.size());

    std::error_code error;
    imif_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("imif", imif_box->type());
}
