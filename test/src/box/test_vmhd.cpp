// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/vmhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_vmhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::vmhd b(parent);
    EXPECT_EQ("vmhd", b.type());
}
