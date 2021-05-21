// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/box/data_box.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy_box1 : petro::box::data_box
{
    dummy_box1(const uint8_t* data, uint64_t size) : data_box(data, size)
    {
    }

    std::string type() const override
    {
        return m_type;
    }
};
}

TEST(box_test_data_box, create)
{
    std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00, 'b', 'o', 'x', ' '};
    auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());
}

TEST(box_test_data_box, type)
{
    auto type = "box ";
    std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00, 'b', 'o', 'x', ' '};
    auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

    std::error_code error;
    box->parse(error);
    ASSERT_FALSE(bool(error));

    EXPECT_EQ(type, box->type());
}

TEST(box_test_data_box, extended_type)
{
    {
        SCOPED_TRACE("no extended type");
        std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00,
                                       'b',  'o',  'x',  ' '};
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_TRUE(box->extended_type().empty());
    }
    {
        SCOPED_TRACE("with extended type");

        auto extended_type = "abcdefghijklmnop";
        std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00, 'u',  'u',
                                       'i',  'd',  0x61, 0x62, 0x63, 0x64,
                                       0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
                                       0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70};
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_EQ("uuid", box->type());
        EXPECT_EQ(extended_type, box->extended_type());
    }
}

TEST(box_test_data_box, size)
{
    {
        SCOPED_TRACE("size zero = whole buffer");

        std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x00,
                                       'b',  'o',  'x',  ' '};
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_EQ(buffer.size(), box->size());
    }
    {
        SCOPED_TRACE("size non-zero and not 1 = first 4 bytes");

        std::vector<uint8_t> buffer = {0x00, 0x00, 0x00, 0x0A, 'b',  'o',
                                       'x',  ' ',  0x00, 0x00, // box ends here
                                       0x00, 0x00, 0x00, 0x00, // excessive data
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00};
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        ASSERT_FALSE(bool(error));

        EXPECT_NE(buffer.size(), box->size());
        EXPECT_EQ(10U, box->size());
    }
    {
        SCOPED_TRACE("size 1 = extended_size");

        std::vector<uint8_t> buffer = {
            0x00, 0x00, 0x00, 0x01, 'b',  'o',  'x',  ' ',  0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        auto box = std::make_shared<dummy_box1>(buffer.data(), buffer.size());

        std::error_code error;
        box->parse(error);
        assert(!error);
        ASSERT_FALSE(bool(error));

        EXPECT_NE(buffer.size(), box->size());
        EXPECT_EQ(16U, box->size());
    }
}
