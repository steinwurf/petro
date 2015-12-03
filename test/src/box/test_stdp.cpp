// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stdp.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stdp, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stdp b(parent);
    EXPECT_EQ("stdp", b.type());
}
