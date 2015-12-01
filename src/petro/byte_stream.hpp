// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <istream>
#include <memory>

namespace petro
{
    // a big endian byte stream used for reading data from an mp4 file.
    class byte_stream
    {
    public:

        byte_stream(const uint8_t* data, uint32_t size);
        byte_stream(byte_stream& bs, uint32_t size);

        void skip(uint32_t bytes);

        uint8_t read_uint8_t();

        int16_t read_int16_t();
        uint16_t read_uint16_t();

        int32_t read_int32_t();
        uint32_t read_uint32_t();

        int64_t read_int64_t();
        uint64_t read_uint64_t();

        std::string read_type();
        double read_fixed_point_1616();
        double read_fixed_point_0230();
        float read_fixed_point_88();

        std::string read_iso639();

        std::string read_time32();
        std::string read_time64();

        uint32_t size() const;
        uint32_t remaining_bytes() const;

    private:

        std::string read_time(uint64_t total_time);

    private:

        const uint8_t* m_data;
        const uint32_t m_size;
        uint32_t m_remaining_bytes;
    };
}
