// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/helper.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <cmath>
#include <iterator>
#include <petro/box/all.hpp>
#include <endian/stream_reader.hpp>
#include <endian/big_endian.hpp>

#include <gtest/gtest.h>

TEST(test_helper, read_type)
{
    std::vector<char> data =
        {
            't', 'e', 's', 't',
            's', 'h', 'o', ' ',
            'f', 'u', 'l', 'l',
            'x', 'x', 'x', 'x',
            'a', 'b', 'c', 'd',
        };

    std::vector<std::string> expected =
        {
            "test",
            "sho ",
            "full",
            "xxxx",
            "abcd",
        };

    endian::stream_reader<endian::big_endian> bs(
        (uint8_t*)data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        uint32_t v;
        bs.read(v);
        EXPECT_EQ(expected[i], petro::helper::type(v));
    }
    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_helper, read_fixed_point_1616)
{
    // the test data for this test is created based on an implementation which
    // is thought to be correct.
    // The point of this test is to get notified if the result changes.
    std::vector<uint8_t> data =
        {
            0x00, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF
        };

    std::vector<double> expected =
        {
            0.0,
            1 << 16
        };

    endian::stream_reader<endian::big_endian> bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        uint32_t v;
        bs.read(v);
        double diff =
            std::abs(expected[i] - petro::helper::fixed_point_1616(v));
        EXPECT_TRUE(diff < 0.01);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_helper, read_fixed_point_0230)
{
    // the test data for this test is created based on an implementation which
    // is thought to be correct.
    // The point of this test is to get notified if the result changes.
    std::vector<uint8_t> data =
        {
            0x00, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF
        };

    std::vector<double> expected =
        {
            0.0,
            4.0 // ?
        };

    endian::stream_reader<endian::big_endian> bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        uint32_t v;
        bs.read(v);
        double diff =
            std::abs(expected[i] - petro::helper::fixed_point_0230(v));
        EXPECT_TRUE(diff < 0.01);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_helper, read_fixed_point_88)
{
    // the test data for this test is created based on an implementation which
    // is thought to be correct.
    // The point of this test is to get notified if the result changes.
    std::vector<uint8_t> data =
        {
            0x00, 0x00,
            0xFF, 0xFF
        };

    std::vector<float> expected =
        {
            0.0f,
            255.99609f // ~(1 << 8)
        };

    endian::stream_reader<endian::big_endian> bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 2; ++i)
    {
        uint16_t v;
        bs.read(v);
        double diff = std::abs(expected[i] - petro::helper::fixed_point_88(v));
        EXPECT_TRUE(diff < 0.01);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_helper, read_iso639)
{
    std::vector<uint8_t> data =
        {
            0x55, 0xC4,
            0x1C, 0xB2
        };

    std::vector<std::string> expected =
        {
            "und",
            "ger"
        };

    endian::stream_reader<endian::big_endian> bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 2; ++i)
    {
        uint16_t v;
        bs.read(v);
        EXPECT_EQ(expected[i], petro::helper::iso639(v));
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_helper, read_time32)
{
    std::vector<uint8_t> data =
        {
            0x7C, 0x25, 0xB0, 0x80,
            0x00, 0x00, 0x00, 0x00,
            0xCA, 0x8C, 0xBA, 0x6E,
            0xCA, 0x8C, 0xBA, 0x6F
        };

    std::vector<std::string> expected =
        {
            "1970-01-01 00:00:00",
            "before 1970-01-01 00:00:00",
            "2011-09-07 06:06:38",
            "2011-09-07 06:06:39"
        };

    endian::stream_reader<endian::big_endian> bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        uint32_t v;
        bs.read(v);
        EXPECT_EQ(expected[i], petro::helper::time32(v));
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_helper, read_time64)
{
    std::vector<uint8_t> data =
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x7C, 0x25, 0xB0, 0x80,
            0x00, 0x00, 0x00, 0x00, 0xCA, 0x8C, 0xBA, 0x6E,
            0x00, 0x00, 0x00, 0x00, 0xCA, 0x8C, 0xBA, 0x6F
        };

    std::vector<std::string> expected =
        {
            "before 1970-01-01 00:00:00",
            "1970-01-01 00:00:00",
            "2011-09-07 06:06:38",
            "2011-09-07 06:06:39",
        };

    endian::stream_reader<endian::big_endian> bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 8; ++i)
    {
        uint64_t v;
        bs.read(v);
        EXPECT_EQ(expected[i], petro::helper::time64(v));
    }

    EXPECT_EQ(0U, bs.remaining_size());
}
