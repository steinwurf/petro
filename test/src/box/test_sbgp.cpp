// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/sbgp.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_sbgp, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::sbgp b(parent);
    EXPECT_EQ("sbgp", b.type());
}
