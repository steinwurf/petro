// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/iloc.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_iloc, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::iloc b(parent);
    EXPECT_EQ("iloc", b.type());
}
