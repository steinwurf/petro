// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mehd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mehd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mehd b(parent);
    EXPECT_EQ("mehd", b.type());
}
