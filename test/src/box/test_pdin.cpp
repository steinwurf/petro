// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/pdin.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_pdin, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::pdin b(parent);
    EXPECT_EQ("pdin", b.type());
}
