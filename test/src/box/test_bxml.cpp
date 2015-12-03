// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/bxml.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_bxml, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::bxml b(parent);
    EXPECT_EQ("bxml", b.type());
}
