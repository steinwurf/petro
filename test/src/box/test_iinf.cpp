// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/iinf.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_iinf, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::iinf b(parent);
    EXPECT_EQ("iinf", b.type());
}
