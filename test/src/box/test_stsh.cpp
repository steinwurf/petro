// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stsh.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stsh, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stsh b(parent);
    EXPECT_EQ("stsh", b.type());
}
