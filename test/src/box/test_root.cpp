// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/root.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_root, construct)
{
    petro::box::root r;
    EXPECT_EQ("root", r.type());
}
