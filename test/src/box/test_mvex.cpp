// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mvex.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mvex, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mvex<petro::parser<>> b(parent);
    EXPECT_EQ("mvex", b.type());
}
