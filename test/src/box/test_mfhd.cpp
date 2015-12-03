// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mfhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mfhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mfhd b(parent);
    EXPECT_EQ("mfhd", b.type());
}
