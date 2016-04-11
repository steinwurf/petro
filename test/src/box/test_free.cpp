// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/free.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_free, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::free b(parent);
    EXPECT_EQ("free", b.type());
}
