// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/unknown.hpp>
#include <petro/box/box.hpp>

#include <gtest/gtest.h>

#include <memory>

TEST(box_test_unknown, construct)
{
    std::string type = "unknown";
    petro::box::unknown b(type, std::weak_ptr<petro::box::box>());
    EXPECT_EQ(type + "?", b.type());
}

TEST(box_test_unknown, describe)
{
    auto type = "test";
    petro::box::unknown b(type, std::weak_ptr<petro::box::box>());

    auto description = b.describe();

    auto found_type = description.find(type);
    EXPECT_NE(std::string::npos, found_type);
    auto found_question_mark = description.find("?");
    EXPECT_NE(std::string::npos, found_question_mark);
}

TEST(box_test_unknown, get_child)
{
    auto parent = std::make_shared<petro::box::box>(
                      "parent", std::weak_ptr<petro::box::box>());

    auto child_type = "child";
    auto child = std::make_shared<petro::box::unknown>(child_type, parent);
    parent->add_child(child);

    EXPECT_NE(nullptr, parent->get_child(child_type));
}
