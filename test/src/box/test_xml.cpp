// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/xml.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_xml, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::xml b(parent);
    EXPECT_EQ("xml ", b.type());
}
