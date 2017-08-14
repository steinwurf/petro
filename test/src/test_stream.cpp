// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/stream.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <cmath>
#include <iterator>
#include <petro/box/all.hpp>

#include <gtest/gtest.h>

namespace
{
template<class ValueType>
void test_read(
    petro::stream& bs,
    const std::vector<ValueType>& expected_values)
{
    // check vitals
    EXPECT_EQ(expected_values.size() * sizeof(ValueType), bs.remaining_size());

    for (auto expected_value : expected_values)
    {
        ValueType out = 0;
        std::error_code error;
        bs.read(out, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(expected_value, out);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}
}

TEST(test_stream, create)
{
    uint32_t size = 42;
    uint8_t content = 8;
    std::vector<uint8_t> data(size, content);
    auto bs = petro::stream(data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_size());

    // check contents
    uint64_t last_remaining_size = bs.remaining_size();
    while (bs.remaining_size() != 0)
    {
        uint8_t out = 0;
        std::error_code error;
        bs.read(out, error);
        ASSERT_FALSE(bool(error));

        EXPECT_EQ(content, out);
        EXPECT_NE(last_remaining_size, bs.remaining_size());
        last_remaining_size = bs.remaining_size();
    }
}

TEST(test_stream, skip)
{
    uint32_t remaining_size = 5;
    uint32_t size = 42;
    uint8_t content = 8;
    std::vector<uint8_t> data(size, content);
    auto bs = petro::stream(data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_size());

    std::error_code error;
    bs.skip(size - remaining_size, error);
    ASSERT_FALSE(bool(error));

    // check vitals after skip
    EXPECT_EQ(remaining_size, bs.remaining_size());
}

TEST(test_stream, read_uint8_t)
{
    std::vector<uint8_t> data =
        {
            0xFF,
            0x00,
            0x37,
            0xAF,
            0xC4
        };
    auto expected = data;

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, read_int16_t)
{
    std::vector<uint8_t> data =
        {
            0xFF, 0x00,
            0x37, 0xAF,
            0xAF, 0x37,
            0x00, 0xC4,
            0xC4, 0xFF
        };

    std::vector<int16_t> expected =
        {
            -256,   // hex = 0xFF00
            14255,  // hex = 0x37AF
            -20681, // hex = 0xAF37
            196,    // hex = 0x00C4
            -15105  // hex = 0xC4FF
        };

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, read_uint16_t)
{
    std::vector<uint8_t> data =
        {
            0xFF, 0x00,
            0x37, 0xAF,
            0xAF, 0x37,
            0x00, 0xC4,
            0xC4, 0xFF
        };

    std::vector<uint16_t> expected =
        {
            0xFF00,
            0x37AF,
            0xAF37,
            0x00C4,
            0xC4FF
        };

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, read_int32_t)
{
    std::vector<uint8_t> data =
        {
            0xFF, 0x00, 0x37, 0xAF,
            0xFF, 0x00, 0xAF, 0x37,
            0x37, 0xAF, 0xC4, 0xFF,
            0xAF, 0x37, 0x00, 0xC4,
            0x00, 0xC4, 0x00, 0xC4,
            0xC4, 0x00, 0xC4, 0x00,
            0xC4, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xC4
        };

    std::vector<int32_t> expected =
        {
            -16762961,   // hex = 0xFF0037AF
            -16732361,   // hex = 0xFF00AF37
            934266111,   // hex = 0x37AFC4FF
            -1355349820, // hex = 0xAF3700C4
            12845252,    // hex = 0x00C400C4
            -1006582784, // hex = 0xC400C400
            -989855745,  // hex = 0xC4FFFFFF
            -60          // hex = 0xFFFFFFC4
        };

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, read_uint32_t)
{
    std::vector<uint8_t> data =
        {
            0xFF, 0x00, 0x37, 0xAF,
            0xFF, 0x00, 0xAF, 0x37,
            0x37, 0xAF, 0xC4, 0xFF,
            0xAF, 0x37, 0x00, 0xC4,
            0x00, 0xC4, 0x00, 0xC4,
            0xC4, 0x00, 0xC4, 0x00,
            0xC4, 0xFF, 0xFF, 0xFF
        };

    std::vector<uint32_t> expected =
        {
            0xFF0037AF,
            0xFF00AF37,
            0x37AFC4FF,
            0xAF3700C4,
            0x00C400C4,
            0xC400C400,
            0xC4FFFFFF
        };

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, read_int64_t)
{
    std::vector<uint8_t> data =
        {
            0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
            0xFF, 0x00, 0x37, 0xAF, 0x00, 0xC4, 0xF2, 0xC4,
            0xFF, 0xF5, 0xAF, 0x37, 0xC4, 0xFF, 0x33, 0xA8,
            0x37, 0xAF, 0xC4, 0xFF, 0x04, 0x11, 0xD3, 0xD3,
            0xAF, 0x37, 0xF5, 0xC4, 0x00, 0xC4, 0xF2, 0xC4,
            0x00, 0xC4, 0x00, 0xC4, 0x33, 0xA8, 0xF5, 0x00,
            0xC4, 0x00, 0xC4, 0xD3, 0x00, 0xC4, 0xC4, 0xC4,
            0xC4, 0xFF, 0xFF, 0xFF, 0x33, 0xA8, 0x00, 0x34
        };

    std::vector<int64_t> expected =
        {
            -4294967296,          // hex = 0xFFFFFFFF00000000
            -71996369266216252,   // hex = 0xFF0037AF00C4F2C4
            -2903570680695896,    // hex = 0xFFF5AF37C4FF33A8
            4012642392574383059,  // hex = 0x37AFC4FF0411D3D3
            -5820913771177774396, // hex = 0xAF37F5C400C4F2C4
            55169938115589376,    // hex = 0x00C400C433A8F500
            -4323239231745637180, // hex = 0xC400C4D300C4C4C4
            -4251398051666067404, // hex = 0xC4FFFFFF33A80034
        };

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, read_uint64_t)
{
    std::vector<uint8_t> data =
        {
            0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
            0xFF, 0x00, 0x37, 0xAF, 0x00, 0xC4, 0xF2, 0xC4,
            0xFF, 0xF5, 0xAF, 0x37, 0xC4, 0xFF, 0x33, 0xA8,
            0x37, 0xAF, 0xC4, 0xFF, 0x04, 0x11, 0xD3, 0xD3,
            0xAF, 0x37, 0xF5, 0xC4, 0x00, 0xC4, 0xF2, 0xC4,
            0x00, 0xC4, 0x00, 0xC4, 0x33, 0xA8, 0xF5, 0x00,
            0xC4, 0x00, 0xC4, 0xD3, 0x00, 0xC4, 0xC4, 0xC4,
            0xC4, 0xFF, 0xFF, 0xFF, 0x33, 0xA8, 0x00, 0x34
        };

    std::vector<uint64_t> expected =
        {
            0xFFFFFFFF00000000,
            0xFF0037AF00C4F2C4,
            0xFFF5AF37C4FF33A8,
            0x37AFC4FF0411D3D3,
            0xAF37F5C400C4F2C4,
            0x00C400C433A8F500,
            0xC400C4D300C4C4C4,
            0xC4FFFFFF33A80034
        };

    auto bs = petro::stream(data.data(), data.size());
    test_read(bs, expected);
}

TEST(test_stream, remaining_size)
{
    uint32_t size = 100;
    std::vector<uint8_t> data(size);
    auto bs = petro::stream(data.data(), size);
    EXPECT_EQ(size, bs.remaining_size());
    for (uint32_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(size - i, bs.remaining_size());
        uint8_t out = 0;
        std::error_code error;
        bs.read(out, error);
        ASSERT_FALSE(bool(error));
    }
    EXPECT_EQ(0U, bs.remaining_size());
}


TEST(test_stream, read_type)
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

    petro::stream bs((uint8_t*)data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        std::string type;
        std::error_code error;
        bs.read_type(type, error);
        ASSERT_FALSE(bool(error));

        EXPECT_EQ(expected[i], type);
    }
    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_stream, read_fixed_point_1616)
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

    petro::stream bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        double v = 0;
        std::error_code error;
        bs.read_fixed_point_1616(v, error);
        ASSERT_FALSE(bool(error));
        double diff = std::abs(expected[i] - v);
        EXPECT_TRUE(diff < 0.01);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_stream, read_fixed_point_0230)
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

    petro::stream bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        double v = 0;
        std::error_code error;
        bs.read_fixed_point_0230(v, error);
        ASSERT_FALSE(bool(error));
        double diff = std::abs(expected[i] - v);
        EXPECT_TRUE(diff < 0.01);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_stream, read_fixed_point_88)
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

    petro::stream bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 2; ++i)
    {
        float v = 0;
        std::error_code error;
        bs.read_fixed_point_88(v, error);
        ASSERT_FALSE(bool(error));
        float diff = std::abs(expected[i] - v);
        EXPECT_TRUE(diff < 0.01);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_stream, read_iso639)
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

    petro::stream bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 2; ++i)
    {
        std::string v;
        std::error_code error;
        bs.read_iso639(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(expected[i], v);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_stream, read_time32)
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

    petro::stream bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        std::string v;
        std::error_code error;
        bs.read_time32(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(expected[i], v);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}

TEST(test_stream, read_time64)
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

    petro::stream bs(data.data(), data.size());

    for (uint32_t i = 0; i < data.size() / 8; ++i)
    {
        std::string v;
        std::error_code error;
        bs.read_time64(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(expected[i], v);
    }

    EXPECT_EQ(0U, bs.remaining_size());
}
