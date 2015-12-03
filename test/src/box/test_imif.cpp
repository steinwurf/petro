// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/imif.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_imif, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::imif b(parent);
    EXPECT_EQ("imif", b.type());
}
