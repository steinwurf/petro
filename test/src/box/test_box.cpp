// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/box.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_box : petro::box::box
{
    dummy_box(const std::string& type) :
        m_type(type)
    { }

    std::string type() const override
    {
        return m_type;
    }

    std::string describe() const override
    {
        return "description";
    }

    using petro::box::box::m_parent;

private:

    std::string m_type;
};
struct special_box : dummy_box
{
    static const std::string TYPE;

    special_box() :
        dummy_box(TYPE)
    { }

};

const std::string special_box::TYPE = "special";

struct special_box2
{
    static const std::string TYPE;
};
const std::string special_box2::TYPE = "special2";
}

TEST(box_test_data_box, children)
{
    auto parent = std::make_shared<dummy_box>("parent");
    EXPECT_EQ(0U, parent->children().size());

    auto child_type = "child";
    auto child = std::make_shared<dummy_box>(child_type);
    child->m_parent = parent;
    parent->add_child(child);

    EXPECT_EQ(1U, parent->children().size());

    EXPECT_EQ(parent->type(), child->parent()->type());
    EXPECT_EQ(parent->type(), child->get_parent(parent->type())->type());
    EXPECT_EQ(child->type(), parent->get_child(child->type())->type());

    ASSERT_EQ(1U, parent->get_children(child->type()).size());
    EXPECT_EQ(child->type(), parent->get_children(child->type())[0]->type());
}

TEST(box_test_data_box, parent_of_parent)
{
    auto child = std::make_shared<dummy_box>("child");
    auto parent = std::make_shared<dummy_box>("parent");
    auto grand_parent = std::make_shared<dummy_box>("grand_parent");

    grand_parent->add_child(parent);
    parent->m_parent = grand_parent;

    parent->add_child(child);
    child->m_parent = parent;

    ASSERT_NE(nullptr, child->parent());
    EXPECT_EQ("parent", child->parent()->type());
    ASSERT_NE(nullptr, parent->parent());
    EXPECT_EQ("grand_parent", parent->parent()->type());
    ASSERT_NE(nullptr, child->get_parent("grand_parent"));
    EXPECT_EQ("grand_parent", child->get_parent("grand_parent")->type());
}

TEST(box_test_data_box, child_extraction)
{
    auto child = std::make_shared<special_box>();
    auto parent = std::make_shared<dummy_box>("parent");
    auto grand_parent = std::make_shared<dummy_box>("grand_parent");

    grand_parent->add_child(parent);
    parent->m_parent = grand_parent;

    parent->add_child(child);
    child->m_parent = parent;

    ASSERT_NE(nullptr, grand_parent->get_child("parent"));
    EXPECT_EQ("parent", grand_parent->get_child("parent")->type());
    ASSERT_NE(nullptr, grand_parent->get_child(child->type()));
    EXPECT_EQ(child->type(), grand_parent->get_child(child->type())->type());

    EXPECT_EQ(nullptr, grand_parent->get_child<special_box2>());
    ASSERT_NE(nullptr, grand_parent->get_child<special_box>());
    EXPECT_EQ(child->type(), grand_parent->get_child<special_box>()->type());
}

TEST(box_test_data_box, describe)
{
    auto type = "test";
    auto box = std::make_shared<dummy_box>(type);

    auto description = box->describe();
    EXPECT_EQ("description", description);
}
