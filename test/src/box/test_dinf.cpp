// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/dinf.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_dinf, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::dinf<petro::parser<>> b(parent);
    EXPECT_EQ("dinf", b.type());
}
