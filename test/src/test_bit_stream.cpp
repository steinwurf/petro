// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/bit_stream.hpp>

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

TEST(test_bit_stream, test_documentation)
{
    /// Bit reader for reading packed bits.
    /// The reading is done in most significant byte first, most significant
    /// bit first order. Example:
    ///
    /// 5 bits: 4 (00100)
    /// 4 bits: 8 (1000)
    /// 4 bits: 2 (0010)
    /// 3 bits: 0 (000)
    ///
    /// Byte 1: 00100100
    /// Byte 2: 00010000
    ///
    /// 00100100 00010000
    /// [ 2 ][ 4 ][ 2][0]
    ///
    std::vector<uint8_t> data{
        0x24, // 00100100
        0x10, // 00010000
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(4U, v);

    bits.read_bits(v, 4, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(8U, v);

    bits.read_bits(v, 4, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(2U, v);

    bits.read_bits(v, 3, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v);
}

TEST(test_bit_stream, read_bit)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0xA5, // 1010 0101
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    {
        SCOPED_TRACE("nibble 1");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 2");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 3");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 4");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 5");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 6");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 7");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 8");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 9");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 10");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
}

TEST(test_bit_stream, read_bits_3)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0x36  // 0011 0110
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    {
        SCOPED_TRACE("nibble 1");
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 2");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 3");
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(7U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 4");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(7U, v);
    }
    {
        SCOPED_TRACE("nibble 5");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(7U, v);
    }
    {
        SCOPED_TRACE("nibble 6");
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 7");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 8");
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(7U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 9");
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
    }
    {
        SCOPED_TRACE("nibble 10");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(6U, v);
    }
}

TEST(test_bit_stream, read_bits_4)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0x36, // 0011 0110
        0xAA  // 1010 1010
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    {
        SCOPED_TRACE("nibble 1");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 2");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 3");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(15U, v);
    }
    {
        SCOPED_TRACE("nibble 4");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(15U, v);
    }
    {
        SCOPED_TRACE("nibble 5");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(15U, v);
    }
    {
        SCOPED_TRACE("nibble 6");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 7");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(0U, v);
    }
    {
        SCOPED_TRACE("nibble 8");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(15U, v);
    }
    {
        SCOPED_TRACE("nibble 9");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(3U, v);
    }
    {
        SCOPED_TRACE("nibble 10");
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(6U, v);
    }
    {
        SCOPED_TRACE("last byte");
        bits.read_bit(v, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(1U, v);
        bits.read_bits(v, 4, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(5U, v);
        bits.read_bits(v, 3, error);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(2U, v);
    }
}

TEST(test_bit_stream, read_bits_5)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0x36  // 0011 0110
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v); // 0000 0

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(3U, v); // 000 11

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(31U, v); // 11 111

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(31U, v); // 1 1111

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v); // 0000 0

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(3U, v); // 000 11

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(25U, v); // 11 001

    bits.read_bits(v, 5, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(22U, v); // 1 0110
}

TEST(test_bit_stream, read_bits_6)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0x36, // 0011 0110
        0x30  // 0011 0000
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v); // 0000 00

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(15U, v); // 00 1111

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(63U, v); // 1111 11

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(48U, v); // 11 0000

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(3U, v); // 0000 11

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(51U, v); // 11 0011

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(24U, v); // 0110 00

    bits.read_bits(v, 6, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(48U, v); // 11 0000
}

TEST(test_bit_stream, read_bits_7)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0x36, // 0011 0110
        0x30, // 0011 0000
        0xDA  // 1101 1010
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v); //  0000 000

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(63U, v); // 0 1111 11

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(126U, v); // 11 1111 0

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v); // 000 0000

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(121U, v); //  1111 001

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(88U, v); // 1 0110 00

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(97U, v); // 11 0000 1

    bits.read_bits(v, 7, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(90U, v); // 101 1010
}

TEST(test_bit_stream, read_bits_8)
{
    std::vector<uint8_t> data{
        0x00, // 0000 0000
        0xFF, // 1111 1111
        0xF0, // 1111 0000
        0x0F, // 0000 1111
        0x36, // 0011 0110
        0x30, // 0011 0000
        0xDA  // 1101 1010
    };

    petro::bit_stream bits(data.data(), data.size());

    std::error_code error;
    uint8_t v = 0;

    bits.read_bits(v, 1, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(0U, v); // 0

    bits.read_bits(v, 8, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(1U, v); // 000 0000 1

    bits.read_bits(v, 8, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(255U, v); // 111 1111 1

    bits.read_bits(v, 8, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(224U, v); // 111 0000 0

    bits.read_bits(v, 8, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(30U, v); // 000 1111 0

    bits.read_bits(v, 8, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(108U, v); // 011 0110 0

    bits.read_bits(v, 8, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(97U, v); // 011 0000 1

    bits.read_bits(v, 3, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(5U, v); // 101

    bits.read_bits(v, 2, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(2U, v); // 10

    bits.read_bits(v, 2, error);
    ASSERT_FALSE(bool(error));
    EXPECT_EQ(2U, v); // 10
}
