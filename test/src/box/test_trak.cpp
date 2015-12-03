// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/trak.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_trak, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::trak<petro::parser<>> b(parent);
    EXPECT_EQ("trak", b.type());
}
