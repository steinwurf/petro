// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/hdlr.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_hdlr, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::hdlr b(parent);
    EXPECT_EQ("hdlr", b.type());
}
