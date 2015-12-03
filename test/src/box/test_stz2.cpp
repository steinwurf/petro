// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stz2.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stz2, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stz2 b(parent);
    EXPECT_EQ("stz2", b.type());
}
