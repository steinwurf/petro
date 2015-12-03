// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stts.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stts, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stts b(parent);
    EXPECT_EQ("stts", b.type());
}
