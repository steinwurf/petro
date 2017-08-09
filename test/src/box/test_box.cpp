// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/box.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_box1 : petro::box::box
{
    dummy_box1(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    std::string type() const override
    {
        return m_type;
    }
};
struct dummy_box2 : petro::box::box
{
    dummy_box2(const std::string& type) :
        box((uint8_t*)0x12345678, 1U)
    {
        m_type = type;
    }

    std::string type() const override
    {
        return m_type;
    }

private:

    using box::parse;
};
}

TEST(box_test_box, create)
{
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'b', 'o', 'x', ' '
    };
    auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());
}

TEST(box_test_box, type)
{
    auto type = "box ";
    std::vector<uint8_t> buffer =
    {
        0x00, 0x00, 0x00, 0x00,
        'b', 'o', 'x', ' '
    };
    auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

    std::error_code error;
    box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ(type, box->type());
}

TEST(box_test_box, extended_type)
{
    {
        SCOPED_TRACE("no extended type");
        std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'b', 'o', 'x', ' '
        };
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_TRUE(box->extended_type().empty());
    }
    {
        SCOPED_TRACE("with extended type");

        auto extended_type = "abcdefghijklmnop";
        std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'u', 'u', 'i', 'd',
            0x61, 0x62, 0x63, 0x64,
            0x65, 0x66, 0x67, 0x68,
            0x69, 0x6a, 0x6b, 0x6c,
            0x6d, 0x6e, 0x6f, 0x70
        };
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_EQ("uuid", box->type());
        EXPECT_EQ(extended_type, box->extended_type());
    }
}

TEST(box_test_box, size)
{
    {
        SCOPED_TRACE("size zero = whole buffer");

        std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x00,
            'b', 'o', 'x', ' '
        };
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_EQ(buffer.size(), box->size());
    }
    {
        SCOPED_TRACE("size non-zero and not 1 = first 4 bytes");

        std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x0A,
            'b', 'o', 'x', ' ',
            0x00, 0x00,  // box ends here
            0x00, 0x00, 0x00, 0x00, // excessive data
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_NE(buffer.size(), box->size());
        EXPECT_EQ(10U, box->size());
    }
    {
        SCOPED_TRACE("size 1 = extended_size");

        std::vector<uint8_t> buffer =
        {
            0x00, 0x00, 0x00, 0x01,
            'b', 'o', 'x', ' ',
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x10,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        assert(!error);
        ASSERT_FALSE(bool(error));

        EXPECT_NE(buffer.size(), box->size());
        EXPECT_EQ(16U, box->size());
    }
}

TEST(box_test_box, children)
{
    auto parent = std::make_shared<dummy_box2>("parent");
    EXPECT_EQ(0U, parent->children().size());

    auto child_type = "child";
    auto child = std::make_shared<dummy_box2>(child_type);
    child->set_parent(parent);
    parent->add_child(child);

    EXPECT_EQ(1U, parent->children().size());

    EXPECT_EQ(parent->type(), child->parent()->type());
    EXPECT_EQ(parent->type(), child->get_parent(parent->type())->type());
    EXPECT_EQ(child->type(), parent->get_child(child->type())->type());

    EXPECT_EQ(1U, parent->get_children(child->type()).size());
    EXPECT_EQ(child->type(), parent->get_children(child->type())[0]->type());
}

TEST(box_test_box, describe)
{
    auto type = "test";
    auto box = std::make_shared<dummy_box2>(type);

    auto description = box->describe();

    auto found = description.find(type);
    EXPECT_NE(std::string::npos, found);
}
