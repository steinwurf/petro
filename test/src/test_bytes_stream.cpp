// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/byte_stream.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>
#include <arpa/inet.h>
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
    uint32_t last_remaing_bytes = bs.remaining_bytes();
    while(bs.remaining_bytes() != 0)
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
    uint32_t last_remaing_bytes1 = bs1.remaining_bytes();
    while(bs1.remaining_bytes() != 0)
    {
        EXPECT_EQ(content, bs1.read_uint8_t());
        EXPECT_NE(last_remaing_bytes1, bs1.remaining_bytes());
        last_remaing_bytes1 = bs1.remaining_bytes();
    }

    uint32_t last_remaing_bytes2 = bs2.remaining_bytes();
    while(bs2.remaining_bytes() != 0)
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
    std::vector<uint8_t> data = {0xFF, 0x00, 0x37, 0xAF, 0xC4};
    auto expected = data;

    // create byte_stream
    auto bs = petro::byte_stream(data.data(), data.size());

    // check vitals
    EXPECT_EQ(data.size(), bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_uint8_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_int16_t)
{
    std::vector<uint8_t> data = {
        0xFF, 0x00,
        0x37, 0xAF,
        0xAF, 0x37,
        0x00, 0xC4,
        0xC4, 0xFF
    };
    std::vector<uint8_t> expected = {
        0x00, 0xFF,
        0xAF, 0x37,
        0x37, 0xAF,
        0xC4, 0x00,
        0xFF, 0xC4
    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(int16_t); ++i)
    {
        EXPECT_EQ(((int16_t*)expected.data())[i], bs.read_int16_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint16_t)
{
    std::vector<uint8_t> data = {
        0xFF, 0x00,
        0x37, 0xAF,
        0xAF, 0x37,
        0x00, 0xC4,
        0xC4, 0xFF
    };
    std::vector<uint8_t> expected = {
        0x00, 0xFF,
        0xAF, 0x37,
        0x37, 0xAF,
        0xC4, 0x00,
        0xFF, 0xC4
    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint16_t); ++i)
    {
        EXPECT_EQ(((uint16_t*)expected.data())[i], bs.read_uint16_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_int32_t)
{
    std::vector<uint8_t> data = {
        0xFF, 0x00, 0x37, 0xAF,
        0xFF, 0x00, 0xAF, 0x37,
        0x37, 0xAF, 0xC4, 0xFF,
        0xAF, 0x37, 0x00, 0xC4,
        0x00, 0xC4, 0x00, 0xC4,
        0xC4, 0x00, 0xC4, 0x00,
        0xC4, 0xFF, 0xFF, 0xFF
    };
    std::vector<uint8_t> expected = {
        0xAF, 0x37, 0x00, 0xFF,
        0x37, 0xAF, 0x00, 0xFF,
        0xFF, 0xC4, 0xAF, 0x37,
        0xC4, 0x00, 0x37, 0xAF,
        0xC4, 0x00, 0xC4, 0x00,
        0x00, 0xC4, 0x00, 0xC4,
        0xFF, 0xFF, 0xFF, 0xC4
    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(int32_t); ++i)
    {
        EXPECT_EQ(((int32_t*)expected.data())[i], bs.read_int32_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint32_t)
{
    std::vector<uint8_t> data = {
        0xFF, 0x00, 0x37, 0xAF,
        0xFF, 0x00, 0xAF, 0x37,
        0x37, 0xAF, 0xC4, 0xFF,
        0xAF, 0x37, 0x00, 0xC4,
        0x00, 0xC4, 0x00, 0xC4,
        0xC4, 0x00, 0xC4, 0x00,
        0xC4, 0xFF, 0xFF, 0xFF
    };
    std::vector<uint8_t> expected = {
        0xAF, 0x37, 0x00, 0xFF,
        0x37, 0xAF, 0x00, 0xFF,
        0xFF, 0xC4, 0xAF, 0x37,
        0xC4, 0x00, 0x37, 0xAF,
        0xC4, 0x00, 0xC4, 0x00,
        0x00, 0xC4, 0x00, 0xC4,
        0xFF, 0xFF, 0xFF, 0xC4
    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint32_t); ++i)
    {
        EXPECT_EQ(((uint32_t*)expected.data())[i], bs.read_uint32_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_int64_t)
{
    std::vector<uint8_t> data = {
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0x00, 0x37, 0xAF, 0x00, 0xC4, 0xF2, 0xC4,
        0xFF, 0xF5, 0xAF, 0x37, 0xC4, 0xFF, 0x33, 0xA8,
        0x37, 0xAF, 0xC4, 0xFF, 0x04, 0x11, 0xD3, 0xD3,
        0xAF, 0x37, 0xF5, 0xC4, 0x00, 0xC4, 0xF2, 0xC4,
        0x00, 0xC4, 0x00, 0xC4, 0x33, 0xA8, 0xF5, 0x00,
        0xC4, 0x00, 0xC4, 0xD3, 0x00, 0xC4, 0xC4, 0xC4,
        0xC4, 0xFF, 0xFF, 0xFF, 0x33, 0xA8, 0x00, 0x34
    };
    std::vector<uint8_t> expected = {
        0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xC4, 0xF2, 0xC4, 0x00, 0xAF, 0x37, 0x00, 0xFF,
        0xA8, 0x33, 0xFF, 0xC4, 0x37, 0xAF, 0xF5, 0xFF,
        0xD3, 0xD3, 0x11, 0x04, 0xFF, 0xC4, 0xAF, 0x37,
        0xC4, 0xF2, 0xC4, 0x00, 0xC4, 0xF5, 0x37, 0xAF,
        0x00, 0xF5, 0xA8, 0x33, 0xC4, 0x00, 0xC4, 0x00,
        0xC4, 0xC4, 0xC4, 0x00, 0xD3, 0xC4, 0x00, 0xC4,
        0x34, 0x00, 0xA8, 0x33, 0xFF, 0xFF, 0xFF, 0xC4,

    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(int64_t); ++i)
    {
        EXPECT_EQ(((int64_t*)expected.data())[i], bs.read_int64_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_uint64_t)
{
    std::vector<uint8_t> data = {
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0x00, 0x37, 0xAF, 0x00, 0xC4, 0xF2, 0xC4,
        0xFF, 0xF5, 0xAF, 0x37, 0xC4, 0xFF, 0x33, 0xA8,
        0x37, 0xAF, 0xC4, 0xFF, 0x04, 0x11, 0xD3, 0xD3,
        0xAF, 0x37, 0xF5, 0xC4, 0x00, 0xC4, 0xF2, 0xC4,
        0x00, 0xC4, 0x00, 0xC4, 0x33, 0xA8, 0xF5, 0x00,
        0xC4, 0x00, 0xC4, 0xD3, 0x00, 0xC4, 0xC4, 0xC4,
        0xC4, 0xFF, 0xFF, 0xFF, 0x33, 0xA8, 0x00, 0x34
    };
    std::vector<uint8_t> expected = {
        0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xC4, 0xF2, 0xC4, 0x00, 0xAF, 0x37, 0x00, 0xFF,
        0xA8, 0x33, 0xFF, 0xC4, 0x37, 0xAF, 0xF5, 0xFF,
        0xD3, 0xD3, 0x11, 0x04, 0xFF, 0xC4, 0xAF, 0x37,
        0xC4, 0xF2, 0xC4, 0x00, 0xC4, 0xF5, 0x37, 0xAF,
        0x00, 0xF5, 0xA8, 0x33, 0xC4, 0x00, 0xC4, 0x00,
        0xC4, 0xC4, 0xC4, 0x00, 0xD3, 0xC4, 0x00, 0xC4,
        0x34, 0x00, 0xA8, 0x33, 0xFF, 0xFF, 0xFF, 0xC4,

    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / sizeof(uint64_t); ++i)
    {
        EXPECT_EQ(((uint64_t*)expected.data())[i], bs.read_uint64_t());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_type)
{
    std::vector<char> data = {
        't', 'e', 's', 't',
        's', 'h', 'o', ' ',
        'f', 'u', 'l', 'l',
        'x', 'x', 'x', 'x',
        'a', 'b', 'c', 'd',
    };

    std::vector<std::string> expected = {
        "test",
        "sho ",
        "full",
        "xxxx",
        "abcd",
    };

    // create byte_stream
    auto size = data.size();
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size() / 4; ++i)
    {
        EXPECT_EQ(expected[i], bs.read_type());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_fixed_point_1616)
{
    std::vector<uint32_t> data = {
        0x00000000,
        0xFFFFFFFF
    };

    std::vector<double> expected = {
        0.0,
        1 << 16
    };

    // create byte_stream
    auto size = data.size() * sizeof(uint32_t);
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_FLOAT_EQ(expected[i], bs.read_fixed_point_1616());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_fixed_point_0230)
{
    std::vector<uint32_t> data = {
        0x00000000,
        0xFFFFFFFF
    };

    std::vector<double> expected = {
        0.0,
        4.0
    };

    // create byte_stream
    auto size = data.size() * sizeof(uint32_t);
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_FLOAT_EQ(expected[i], bs.read_fixed_point_0230());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_fixed_point_88)
{
    std::vector<uint16_t> data = {
        0x0000,
        0xFFFF
    };

    std::vector<float> expected = {
        0.0,
        255.99609 // ~(1 << 8)
    };

    // create byte_stream
    auto size = data.size() * sizeof(uint16_t);
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_FLOAT_EQ(expected[i], bs.read_fixed_point_88());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_iso639)
{
    std::vector<uint16_t> data = {
        0xC455,
        0xB21C
    };

    std::vector<std::string> expected = {
        "und",
        "ger"
    };

    // create byte_stream
    auto size = data.size() * sizeof(uint16_t);
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_iso639());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_time32)
{
    std::vector<uint32_t> data = {
        0x6EBA8CCA,
        0x6FBA8CCA
    };

    std::vector<std::string> expected = {
        "Wed Sep  7 08:06:38 2011",
        "Wed Sep  7 08:06:39 2011"
    };

    // create byte_stream
    auto size = data.size() * sizeof(uint32_t);
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_time32());
    }

    EXPECT_EQ(0U, bs.remaining_bytes());
}

TEST(test_byte_stream, read_time64)
{
    std::vector<uint64_t> data = {
        0x6EBA8CCA00000000,
        0x6FBA8CCA00000000
    };

    std::vector<std::string> expected = {
        "Wed Sep  7 08:06:38 2011",
        "Wed Sep  7 08:06:39 2011"
    };

    // create byte_stream
    auto size = data.size() * sizeof(uint64_t);
    auto bs = petro::byte_stream((uint8_t*)data.data(), size);

    // check vitals
    EXPECT_EQ(size, bs.remaining_bytes());

    for (uint32_t i = 0; i < data.size(); ++i)
    {
        EXPECT_EQ(expected[i], bs.read_time64());
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
