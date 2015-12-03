// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/subs.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_subs, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::subs b(parent);
    EXPECT_EQ("subs", b.type());
}
