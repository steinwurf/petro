// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stss.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stss, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stss b(parent);
    EXPECT_EQ("stss", b.type());
}
