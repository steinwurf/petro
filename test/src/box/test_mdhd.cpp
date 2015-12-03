// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mdhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mdhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mdhd b(parent);
    EXPECT_EQ("mdhd", b.type());
}
