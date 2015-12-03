// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/sinf.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_sinf, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::sinf<petro::parser<>> b(parent);
    EXPECT_EQ("sinf", b.type());
}
