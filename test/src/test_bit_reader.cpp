// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/bit_reader.hpp>

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

TEST(test_bit_reader, read_bit)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0xA5, //1010 0101
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    {
        SCOPED_TRACE("nibble 1");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(1U, bits.read_bit());
    }
}


TEST(test_bit_reader, read_bits_2)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36  //0011 0110
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    {
        SCOPED_TRACE("nibble 1");
        EXPECT_EQ(0U, bits.read_bits(2));
        EXPECT_EQ(0U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0U, bits.read_bits(2));
        EXPECT_EQ(0U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(3U, bits.read_bits(2));
        EXPECT_EQ(3U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(3U, bits.read_bits(2));
        EXPECT_EQ(3U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(3U, bits.read_bits(2));
        EXPECT_EQ(3U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0U, bits.read_bits(2));
        EXPECT_EQ(0U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0U, bits.read_bits(2));
        EXPECT_EQ(0U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(3U, bits.read_bits(2));
        EXPECT_EQ(3U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(0U, bits.read_bits(2));
        EXPECT_EQ(3U, bits.read_bits(2));
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(1U, bits.read_bits(2));
        EXPECT_EQ(2U, bits.read_bits(2));
    }
}

TEST(test_bit_reader, read_bits_3)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36  //0011 0110
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    {
        SCOPED_TRACE("nibble 1");
        EXPECT_EQ(0U, bits.read_bits(3));
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bits(3));
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(7U, bits.read_bits(3));
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(7U, bits.read_bits(3));
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(7U, bits.read_bits(3));
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0U, bits.read_bits(3));
        EXPECT_EQ(0U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(0U, bits.read_bits(3));
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(7U, bits.read_bits(3));
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(1U, bits.read_bits(3));
        EXPECT_EQ(1U, bits.read_bit());
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(0U, bits.read_bit());
        EXPECT_EQ(6U, bits.read_bits(3));
    }
}

TEST(test_bit_reader, read_bits_4)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36, //0011 0110
        0xAA  //1010 1010
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    {
        SCOPED_TRACE("nibble 1");
        EXPECT_EQ(0U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(15U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(15U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(15U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(15U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(3U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(6U, bits.read_bits(4));
    }
    {
        SCOPED_TRACE("last byte");
        EXPECT_EQ(1U, bits.read_bit());
        EXPECT_EQ(5U, bits.read_bits(4));
        EXPECT_EQ(2U, bits.read_bits(3));
    }
}

TEST(test_bit_reader, read_bits_5)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36  //0011 0110
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    EXPECT_EQ(0U, bits.read_bits(5));  // 0000 0
    EXPECT_EQ(3U, bits.read_bits(5));  // 000 11
    EXPECT_EQ(31U, bits.read_bits(5)); // 11 111
    EXPECT_EQ(31U, bits.read_bits(5)); // 1 1111
    EXPECT_EQ(0U, bits.read_bits(5));  // 0000 0
    EXPECT_EQ(3U, bits.read_bits(5));  // 000 11
    EXPECT_EQ(25U, bits.read_bits(5)); // 11 001
    EXPECT_EQ(22U, bits.read_bits(5)); // 1 0110
}

TEST(test_bit_reader, read_bits_6)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36, //0011 0110
        0x30  //0011 0000
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    EXPECT_EQ(0U, bits.read_bits(6));  // 0000 00
    EXPECT_EQ(15U, bits.read_bits(6)); // 00 1111
    EXPECT_EQ(63U, bits.read_bits(6)); // 1111 11
    EXPECT_EQ(48U, bits.read_bits(6)); // 11 0000
    EXPECT_EQ(3U, bits.read_bits(6));  // 0000 11
    EXPECT_EQ(51U, bits.read_bits(6)); // 11 0011
    EXPECT_EQ(24U, bits.read_bits(6)); // 0110 00
    EXPECT_EQ(48U, bits.read_bits(6)); // 11 0000
}

TEST(test_bit_reader, read_bits_7)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36, //0011 0110
        0x30, //0011 0000
        0xDA  //1101 1010
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    EXPECT_EQ(0U, bits.read_bits(7));   //  0000 000
    EXPECT_EQ(63U, bits.read_bits(7));  // 0 1111 11
    EXPECT_EQ(126U, bits.read_bits(7)); // 11 1111 0
    EXPECT_EQ(0U, bits.read_bits(7));   // 000 0000
    EXPECT_EQ(121U, bits.read_bits(7)); //  1111 001
    EXPECT_EQ(88U, bits.read_bits(7));  // 1 0110 00
    EXPECT_EQ(97U, bits.read_bits(7));  // 11 0000 1
    EXPECT_EQ(90U, bits.read_bits(7));  // 101 1010
}

TEST(test_bit_reader, read_bits_8)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36, //0011 0110
        0x30, //0011 0000
        0xDA  //1101 1010
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    EXPECT_EQ(0U, bits.read_bit());    // 0
    EXPECT_EQ(1U, bits.read_bits(8));   // 000 0000 1
    EXPECT_EQ(255U, bits.read_bits(8)); // 111 1111 1
    EXPECT_EQ(224U, bits.read_bits(8)); // 111 0000 0
    EXPECT_EQ(30U, bits.read_bits(8));  // 000 1111 0
    EXPECT_EQ(108U, bits.read_bits(8)); // 011 0110 0
    EXPECT_EQ(97U, bits.read_bits(8));  // 011 0000 1
    EXPECT_EQ(5U, bits.read_bits(3));   // 101
    EXPECT_EQ(2U, bits.read_bits(2));   // 10
    EXPECT_EQ(2U, bits.read_bits(2));   // 10
}
