// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/trun.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_trun, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::trun b(parent);
    EXPECT_EQ("trun", b.type());
}
