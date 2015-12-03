// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/stbl.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_stbl, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::stbl<petro::parser<>> b(parent);
    EXPECT_EQ("stbl", b.type());
}
