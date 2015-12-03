// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/sgpd.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_sgpd, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::sgpd b(parent);
    EXPECT_EQ("sgpd", b.type());
}
