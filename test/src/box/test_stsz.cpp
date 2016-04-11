// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/stsz.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stsz, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stsz b(parent);
    EXPECT_EQ("stsz", b.type());
}
