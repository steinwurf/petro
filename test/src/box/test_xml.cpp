// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/xml.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_xml, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'x', 'm', 'l', ' '
        };
    auto xml_box = std::make_shared<petro::box::xml>(
        buffer.data(), buffer.size());

    std::error_code error;
    xml_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("xml ", xml_box->type());
}
