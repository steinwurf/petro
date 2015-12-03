// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/free.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_free, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::free b(parent);
    EXPECT_EQ("free", b.type());
}
