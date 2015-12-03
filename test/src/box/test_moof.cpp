// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/moof.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_moof, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::moof<petro::parser<>> b(parent);
    EXPECT_EQ("moof", b.type());
}
