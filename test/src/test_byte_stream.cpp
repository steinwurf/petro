// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/byte_stream.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <cmath>
#include <iterator>
#include <petro/box/all.hpp>

#include <gtest/gtest.h>

TEST(test_byte_stream, create)
{
    // create byte_stream
    uint32_t size = 42;
    uint8_t content = 8;
    std::vector<uint8_t> data(size, content);
    auto bs = petro::byte_stream(data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    // check contents
    uint64_t last_remaing_bytes = bs.remaining_bytes();
    while (bs.remaining_bytes() != 0)
    {
        EXPECT_EQ(content, bs.read_uint8_t());
        EXPECT_NE(last_remaing_bytes, bs.remaining_bytes());
        last_remaing_bytes = bs.remaining_bytes();
    }
}

TEST(test_byte_stream, create_from_byte_stream)
{
    // create source byte_stream
    uint32_t size = 42;
    uint8_t content = 8;
    std::vector<uint8_t> data(size, content);
    auto bs = petro::byte_stream(data.data(), size);

    // create byte_stream 1 from source byte_stream
    auto bs1 = petro::byte_stream(bs, size / 2);

    // create byte_stream 2 from source byte_stream
    auto bs2 = petro::byte_stream(bs, size / 2);

    // check vitals of byte_stream..
    EXPECT_EQ(0U, bs.remaining_bytes());
    // .. and byte_stream 1 and 2
    EXPECT_EQ(size / 2, bs1.remaining_bytes());
    EXPECT_EQ(size / 2, bs2.remaining_bytes());

    // check content of byte_stream 1 and 2
    uint64_t last_remaing_bytes1 = bs1.remaining_bytes();
    while (bs1.remaining_bytes() != 0)
    {
        EXPECT_EQ(content, bs1.read_uint8_t());
        EXPECT_NE(last_remaing_bytes1, bs1.remaining_bytes());
        last_remaing_bytes1 = bs1.remaining_bytes();
    }

    uint64_t last_remaing_bytes2 = bs2.remaining_bytes();
    while (bs2.remaining_bytes() != 0)
    {
        EXPECT_EQ(content, bs2.read_uint8_t());
        EXPECT_NE(last_remaing_bytes2, bs2.remaining_bytes());
        last_remaing_bytes2 = bs2.remaining_bytes();
    }
}

TEST(test_byte_stream, skip)
{
    // create byte_stream
    uint32_t remaining_bytes = 5;
    uint32_t size = 42;
    uint8_t content = 8;
    std::vector<uint8_t> data(size, content);
    auto bs = petro::byte_stream(data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    bs.skip(size - remaining_bytes);

    // check vitals after skip
    EXPECT_EQ(remaining_bytes, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint8_t)
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

    // create byte_stream
    auto bs = petro::byte_stream(data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint8_t); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_uint8_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_int16_t)
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

    // create byte_stream
    auto bs = petro::byte_stream(data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(int16_t); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_int16_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint16_t)
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

    // create byte_stream
    auto bs = petro::byte_stream(data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint16_t); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_uint16_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_int32_t)
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

    // create byte_stream
    auto bs = petro::byte_stream(data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(int32_t); ++i)
    {
        SCOPED_TRACE(i);
        EXPECT_EQ(expected[i], bs.read_int32_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint32_t)
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

    // create byte_stream
    auto bs = petro::byte_stream((uint8_t*)data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint32_t); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_uint32_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_int64_t)
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

    // create byte_stream
    auto bs = petro::byte_stream((uint8_t*)data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(int64_t); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_int64_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint64_t)
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

    // create byte_stream
    auto bs = petro::byte_stream((uint8_t*)data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint64_t); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_uint64_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, remaining_bytes)
{
    uint32_t size = 100;
    std::vector<uint8_t> data(size);
    auto bs = petro::byte_stream(data.data(), size);
    EXPECT_EQ(size, bs.remaining_bytes());
    for (uint32_t i = 0; i < size; ++i)
    {
        bs.read_uint8_t();
    }
    EXPECT_EQ(0U, bs.remaining_bytes());
}
