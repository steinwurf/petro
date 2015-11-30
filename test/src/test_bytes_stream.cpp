// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include <petro/byte_stream.hpp>

#include <cstdint>
#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <iterator>

#include <petro/box/all.hpp>

#include <gtest/gtest.h>

TEST(test_byte_stream, creation)
{
    uint32_t size = 42;
    std::vector<uint8_t> data(size);
    auto bs = petro::byte_stream(data.data(), size);
    EXPECT_EQ(size, bs.size());
    EXPECT_EQ(size, bs.remaining_bytes());
}
        // byte_stream(const uint8_t* data, uint32_t size);
        // byte_stream(byte_stream& bs, uint32_t size);

        // void skip(uint32_t bytes);

        // uint8_t read_uint8_t();

        // int16_t read_int16_t();
        // uint16_t read_uint16_t();

        // int32_t read_int32_t();
        // uint32_t read_uint32_t();

        // int64_t read_int64_t();
        // uint64_t read_uint64_t();

        // std::string read_type();

        // float read_fixed_point(uint32_t integer_length,
        //     uint32_t fractional_length);

        // std::string read_iso639_code();

        // std::string read_time32();
        // std::string read_time64();

        // uint32_t size() const;
        // uint32_t remaining_bytes() const;