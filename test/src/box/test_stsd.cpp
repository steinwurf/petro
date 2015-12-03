// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stsd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stsd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stsd b(parent);
    EXPECT_EQ("stsd", b.type());
}
