// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/url.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_url, construct)
{
    std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00, 'u',  'r',
                                   'l',  ' ',  0x00, 0x00, 0x00, 0x00};
    auto url_box =
        std::make_shared<petro::box::url>(buffer.data(), buffer.size());

    std::error_code error;
    url_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("url ", url_box->type());
}
