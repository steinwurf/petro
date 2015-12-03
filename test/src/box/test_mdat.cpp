// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mdat.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mdat, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mdat b(parent);
    EXPECT_EQ("mdat", b.type());
}
