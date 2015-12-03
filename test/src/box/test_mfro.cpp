// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mfro.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mfro, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mfro b(parent);
    EXPECT_EQ("mfro", b.type());
}
