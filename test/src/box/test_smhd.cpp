// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/smhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_smhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::smhd b(parent);
    EXPECT_EQ("smhd", b.type());
}
