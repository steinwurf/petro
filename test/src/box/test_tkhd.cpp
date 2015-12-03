// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/tkhd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_tkhd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::tkhd b(parent);
    EXPECT_EQ("tkhd", b.type());
}
