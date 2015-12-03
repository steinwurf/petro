// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/tref.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_tref, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::tref b(parent);
    EXPECT_EQ("tref", b.type());
}
