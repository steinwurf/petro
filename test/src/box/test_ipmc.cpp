// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/ipmc.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_ipmc, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::ipmc b(parent);
    EXPECT_EQ("ipmc", b.type());
}
