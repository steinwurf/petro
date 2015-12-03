// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/ctts.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_ctts, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::ctts b(parent);
    EXPECT_EQ("ctts", b.type());
}
