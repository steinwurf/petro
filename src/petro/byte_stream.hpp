// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <istream>
#include <memory>

namespace petro
{
    class byte_stream
    {
    public:

        byte_stream(const uint8_t* data, uint32_t size);

        void skip(uint32_t bytes);

        uint8_t read_uint8_t();

        int16_t read_int16_t();
        uint16_t read_uint16_t();

        int32_t read_int32_t();
        uint32_t read_uint32_t();

        std::string read_type();
        uint64_t read_uint64_t();
        float read_fixed_point(uint32_t integer_length,
            uint32_t fractional_length);

        std::string read_iso639_code();

        uint32_t size() const;
        const uint8_t* data() const;

    private:

        const uint8_t* m_data;
        uint32_t m_size;
    };
}
