// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/mfra.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_mfra, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::mfra<petro::parser<>> b(parent);
    EXPECT_EQ("mfra", b.type());
}
