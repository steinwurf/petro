// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>
#include <petro/box/schi.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

TEST(box_test_schi, construct)
{
    std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00, 's', 'c', 'h', 'i'};
    auto schi_box =
        std::make_shared<petro::box::schi>(buffer.data(), buffer.size());

    std::error_code error;
    schi_box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ("schi", schi_box->type());
}
