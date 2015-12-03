// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/pdin.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_pdin, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::pdin b(parent);
    EXPECT_EQ("pdin", b.type());
}
