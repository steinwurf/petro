// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stsc.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stsc, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stsc b(parent);
    EXPECT_EQ("stsc", b.type());
}
