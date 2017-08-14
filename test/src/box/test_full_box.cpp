// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/full_box.hpp>
#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

namespace
{
struct dummy_full_box : petro::box::full_box
{
    dummy_full_box(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    std::string type() const override
    {
        return "fbox";
    }
};
}

TEST(box_test_fbtest, construct)
{
    std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'f', 'b', 'o', 'x',
            0x00,
            0x00, 0x00, 0x00
        };

    auto full_box = std::make_shared<dummy_full_box>(
        buffer.data(), buffer.size());

    std::error_code error;
    full_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("fbox", full_box->type());
}
