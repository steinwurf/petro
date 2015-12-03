// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/dref.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_dref, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::dref<petro::parser<>> b(parent);
    EXPECT_EQ("dref", b.type());
}
