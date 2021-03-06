// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/stsc.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_stsc, construct)
{
    std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00, 's',  't',
                                   's',  'c',  0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00};
    auto stsc_box =
        std::make_shared<petro::box::stsc>(buffer.data(), buffer.size());

    std::error_code error;
    stsc_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("stsc", stsc_box->type());
}
