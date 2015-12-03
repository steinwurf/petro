// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/pitm.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_pitm, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::pitm b(parent);
    EXPECT_EQ("pitm", b.type());
}
