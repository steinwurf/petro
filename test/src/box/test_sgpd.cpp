// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/sgpd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_sgpd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::sgpd b(parent);
    EXPECT_EQ("sgpd", b.type());
}
