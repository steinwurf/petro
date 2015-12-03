// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/trex.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_trex, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::trex b(parent);
    EXPECT_EQ("trex", b.type());
}
