// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/tfhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_tfhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::tfhd b(parent);
    EXPECT_EQ("tfhd", b.type());
}
