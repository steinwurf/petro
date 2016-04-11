// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/mdhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mdhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mdhd b(parent);
    EXPECT_EQ("mdhd", b.type());
}
