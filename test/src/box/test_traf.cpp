// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/traf.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_traf, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::traf<petro::parser<>> b(parent);
    EXPECT_EQ("traf", b.type());
}
