// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/unknown.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_unknown, construct)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
         'b',  'o',  'x',  'y'
    };
    auto unknown_box = std::make_shared<petro::box::unknown>(
        buffer.data(), buffer.size());

    std::error_code error;
    unknown_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("boxy", unknown_box->type());

    auto description = unknown_box->describe();
    auto found_question_mark = description.find("unknown");
    EXPECT_NE(std::string::npos, found_question_mark);
}
