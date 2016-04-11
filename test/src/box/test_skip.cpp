// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/skip.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_skip, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::skip b(parent);
    EXPECT_EQ("skip", b.type());
}
