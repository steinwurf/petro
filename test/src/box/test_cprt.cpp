// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/cprt.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_cprt, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::cprt b(parent);
    EXPECT_EQ("cprt", b.type());
}
