// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
