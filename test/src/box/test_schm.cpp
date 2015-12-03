// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/box/schm.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_schm, construct)
{
    std::weak_ptr<petro::box::box> parent;
    petro::box::schm b(parent);
    EXPECT_EQ("schm", b.type());
}
