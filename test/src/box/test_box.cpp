// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/box.hpp>

#include <gtest/gtest.h>

TEST(box_test_box, create)
{
    petro::box::box b("test", std::weak_ptr<petro::box::box>());
}

TEST(box_test_box, type)
{
    auto type = "test";
    petro::box::box b(type, std::weak_ptr<petro::box::box>());
    EXPECT_EQ(type, b.type());
}

TEST(box_test_box, extended_type)
{
    petro::box::box btest("test", std::weak_ptr<petro::box::box>());
    EXPECT_TRUE(btest.extended_type().empty());

    auto extended_type = "abcdefghijklmnop";
    // extended type in big endian hex codes.
    std::vector<uint8_t> data
    {
        0x61,
        0x62,
        0x63,
        0x64,
        0x65,
        0x66,
        0x67,
        0x68,
        0x69,
        0x6a,
        0x6b,
        0x6c,
        0x6d,
        0x6e,
        0x6f,
        0x70
    };
    petro::byte_stream bs(data.data(), data.size());

    petro::box::box buuid("uuid", std::weak_ptr<petro::box::box>());
    buuid.read(24, bs);
    EXPECT_EQ(extended_type, buuid.extended_type());
}

TEST(box_test_box, size)
{
    petro::box::box b("test", std::weak_ptr<petro::box::box>());
    EXPECT_EQ(0U, b.size());


    auto size = 42;
    std::vector<uint8_t> data;
    petro::byte_stream bs(data.data(), size);
    b.read(size, bs);
}

TEST(box_test_box, children)
{
    auto parent_type = "parent";
    auto parent = std::make_shared<petro::box::box>(
                      parent_type, std::weak_ptr<petro::box::box>());
    EXPECT_EQ(0U, parent->children().size());

    auto child_type = "child";
    auto child = std::make_shared<petro::box::box>(
                     child_type, parent);

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
    petro::box::box b(type, std::weak_ptr<petro::box::box>());

    auto description = b.describe();

    auto found = description.find(type);
    EXPECT_NE(std::string::npos, found);
}

TEST(box_test_box, read)
{
    {
        SCOPED_TRACE("default size");
        petro::box::box b("test", std::weak_ptr<petro::box::box>());
        auto box_size = 42U;
        std::vector<uint8_t> data(box_size + 10, 1);
        petro::byte_stream bs(data.data(), data.size());
        b.read(box_size, bs);
        EXPECT_EQ(box_size, b.size());
    }
    {
        SCOPED_TRACE("extended size");
        petro::box::box b("test", std::weak_ptr<petro::box::box>());
        std::vector<uint8_t> data
        {
            // 1337 in big endian hex code.
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x39
        };
        petro::byte_stream bs(data.data(), data.size());
        b.read(1, bs);
        EXPECT_EQ(1337U, b.size());
    }
    {
        SCOPED_TRACE("eof size");
        petro::box::box b("test", std::weak_ptr<petro::box::box>());
        auto byte_stream_size = 42U;
        std::vector<uint8_t> data(byte_stream_size, 1);
        petro::byte_stream bs(data.data(), data.size());
        b.read(0, bs);
        EXPECT_EQ(byte_stream_size, b.size());
    }
}
