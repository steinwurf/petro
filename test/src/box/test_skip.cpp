// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/skip.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_skip, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::skip b(parent);
    EXPECT_EQ("skip", b.type());
}
