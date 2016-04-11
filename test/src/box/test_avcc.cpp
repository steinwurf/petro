// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/avcc.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_avcc, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::avcc b(parent);
    EXPECT_EQ("avcC", b.type());
}
