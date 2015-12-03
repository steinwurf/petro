// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/frma.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_frma, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::frma b(parent);
    EXPECT_EQ("frma", b.type());
}
