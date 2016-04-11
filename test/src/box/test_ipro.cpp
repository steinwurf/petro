// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/ipro.hpp>
#include <petro/box/box.hpp>
#include <petro/parser.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_ipro, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::ipro<petro::parser<>> b(parent);
    EXPECT_EQ("ipro", b.type());
}
