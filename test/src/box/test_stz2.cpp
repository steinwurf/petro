// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stz2.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stz2, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stz2 b(parent);
    EXPECT_EQ("stz2", b.type());
}
