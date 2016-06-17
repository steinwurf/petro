// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <istream>
#include <memory>
#include <cassert>
#include <vector>
#include <fstream>
#include <iostream>

namespace petro
{
    // a big endian byte stream used for reading data from an mp4 file.
    class byte_stream
    {
    public:

        struct i_byte_stream
        {
            virtual uint8_t read_byte() = 0;
            virtual void skip(uint64_t bytes) = 0;
        };

        struct pointer_byte_stream : i_byte_stream
        {
            pointer_byte_stream(const uint8_t* data):
                m_data(data)
            { }

            uint8_t read_byte()
            {
                uint8_t result = m_data[0];
                m_data += 1;
                return result;
            }

            void skip(uint64_t bytes)
            {
                m_data += bytes;
            }

        private:
            const uint8_t* m_data;
        };

        struct file_byte_stream : i_byte_stream
        {
            file_byte_stream(std::ifstream& data):
                m_data(data)
            {
                // @todo probably should be handled differently.
                assert(m_data.good());
            }

            uint8_t read_byte()
            {
                char result;
                m_data.read(&result, 1);
                return result;
            }
            void skip(uint64_t bytes)
            {
                m_data.seekg(bytes, std::ios::cur);
            }

        private:
            std::ifstream& m_data;
        };


    public:

        byte_stream(const uint8_t* data, uint64_t size);
        byte_stream(std::ifstream& data);
        byte_stream(byte_stream& bs, uint64_t size);

        void skip(uint64_t bytes);

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

        uint64_t remaining_bytes() const;

    private:

        std::string read_time(uint64_t total_time);

    private:

        std::shared_ptr<i_byte_stream> m_data;
        uint64_t m_remaining_bytes;
    };
}
