// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/bit_reader.hpp>

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

TEST(test_bit_reader, read_1_bit)
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
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(1, bits.read_1_bit());
    }
}


TEST(test_bit_reader, read_2_bits)
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
        EXPECT_EQ(0, bits.read_2_bits());
        EXPECT_EQ(0, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0, bits.read_2_bits());
        EXPECT_EQ(0, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(3, bits.read_2_bits());
        EXPECT_EQ(3, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(3, bits.read_2_bits());
        EXPECT_EQ(3, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(3, bits.read_2_bits());
        EXPECT_EQ(3, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0, bits.read_2_bits());
        EXPECT_EQ(0, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0, bits.read_2_bits());
        EXPECT_EQ(0, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(3, bits.read_2_bits());
        EXPECT_EQ(3, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(0, bits.read_2_bits());
        EXPECT_EQ(3, bits.read_2_bits());
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(1, bits.read_2_bits());
        EXPECT_EQ(2, bits.read_2_bits());
    }
}

TEST(test_bit_reader, read_3_bits)
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
        EXPECT_EQ(0, bits.read_3_bits());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_3_bits());
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(7, bits.read_3_bits());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(7, bits.read_3_bits());
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(7, bits.read_3_bits());
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0, bits.read_3_bits());
        EXPECT_EQ(0, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(0, bits.read_3_bits());
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(7, bits.read_3_bits());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(1, bits.read_3_bits());
        EXPECT_EQ(1, bits.read_1_bit());
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(0, bits.read_1_bit());
        EXPECT_EQ(6, bits.read_3_bits());
    }
}

TEST(test_bit_reader, read_4_bits)
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
        EXPECT_EQ(0, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 2");
        EXPECT_EQ(0, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 3");
        EXPECT_EQ(15, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 4");
        EXPECT_EQ(15, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 5");
        EXPECT_EQ(15, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 6");
        EXPECT_EQ(0, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 7");
        EXPECT_EQ(0, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 8");
        EXPECT_EQ(15, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 9");
        EXPECT_EQ(3, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("nibble 10");
        EXPECT_EQ(6, bits.read_4_bits());
    }
    {
        SCOPED_TRACE("last byte");
        EXPECT_EQ(1, bits.read_1_bit());
        EXPECT_EQ(5, bits.read_4_bits());
        EXPECT_EQ(2, bits.read_3_bits());
    }
}

TEST(test_bit_reader, read_5_bits)
{
    std::vector<uint8_t> data {
        0x00, //0000 0000
        0xFF, //1111 1111
        0xF0, //1111 0000
        0x0F, //0000 1111
        0x36  //0011 0110
    };

    petro::bit_reader bits(data.data(), data.size() * 8);

    EXPECT_EQ(0, bits.read_5_bits());  // 0000 0
    EXPECT_EQ(3, bits.read_5_bits());  // 000 11
    EXPECT_EQ(31, bits.read_5_bits()); // 11 111
    EXPECT_EQ(31, bits.read_5_bits()); // 1 1111
    EXPECT_EQ(0, bits.read_5_bits());  // 0000 0
    EXPECT_EQ(3, bits.read_5_bits());  // 000 11
    EXPECT_EQ(25, bits.read_5_bits()); // 11 001
    EXPECT_EQ(22, bits.read_5_bits()); // 1 0110
}

TEST(test_bit_reader, read_6_bits)
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

    EXPECT_EQ(0, bits.read_6_bits());  // 0000 00
    EXPECT_EQ(15, bits.read_6_bits()); // 00 1111
    EXPECT_EQ(63, bits.read_6_bits()); // 1111 11
    EXPECT_EQ(48, bits.read_6_bits()); // 11 0000
    EXPECT_EQ(3, bits.read_6_bits());  // 0000 11
    EXPECT_EQ(51, bits.read_6_bits()); // 11 0011
    EXPECT_EQ(24, bits.read_6_bits()); // 0110 00
    EXPECT_EQ(48, bits.read_6_bits()); // 11 0000
}

TEST(test_bit_reader, read_7_bits)
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

    EXPECT_EQ(0, bits.read_7_bits());   //  0000 000
    EXPECT_EQ(63, bits.read_7_bits());  // 0 1111 11
    EXPECT_EQ(126, bits.read_7_bits()); // 11 1111 0
    EXPECT_EQ(0, bits.read_7_bits());   // 000 0000
    EXPECT_EQ(121, bits.read_7_bits()); //  1111 001
    EXPECT_EQ(88, bits.read_7_bits());  // 1 0110 00
    EXPECT_EQ(97, bits.read_7_bits());  // 11 0000 1
    EXPECT_EQ(90, bits.read_7_bits());  // 101 1010
}

TEST(test_bit_reader, read_8_bits)
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

    EXPECT_EQ(0, bits.read_1_bit());    // 0
    EXPECT_EQ(1, bits.read_8_bits());   // 000 0000 1
    EXPECT_EQ(255, bits.read_8_bits()); // 111 1111 1
    EXPECT_EQ(224, bits.read_8_bits()); // 111 0000 0
    EXPECT_EQ(30, bits.read_8_bits());  // 000 1111 0
    EXPECT_EQ(108, bits.read_8_bits()); // 011 0110 0
    EXPECT_EQ(97, bits.read_8_bits());  // 011 0000 1
    EXPECT_EQ(5, bits.read_3_bits());   // 101
    EXPECT_EQ(2, bits.read_2_bits());   // 10
    EXPECT_EQ(2, bits.read_2_bits());   // 10
}
