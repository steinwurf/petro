// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mvhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mvhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mvhd b(parent);
    EXPECT_EQ("mvhd", b.type());
}
