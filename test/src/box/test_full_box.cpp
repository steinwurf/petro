// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/full_box.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_fbtest, construct)
{
    std::weak_ptr<petro::box::box> parent;
    auto type = "fbtest";
    petro::box::full_box fb(type, parent);
    EXPECT_EQ(type, fb.type());
}
