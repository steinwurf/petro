// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stco.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stco, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stco b(parent);
    EXPECT_EQ("stco", b.type());
}
