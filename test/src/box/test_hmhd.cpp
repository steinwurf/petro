// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/hmhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_hmhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::hmhd b(parent);
    EXPECT_EQ("hmhd", b.type());
}
