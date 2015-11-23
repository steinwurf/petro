// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

namespace petro
{
    class flags
    {
    public:

        void read(byte_stream& bs)
        {
            // read 24 bytes
            m_data.push_back(bs.read_uint8_t());
            m_data.push_back(bs.read_uint8_t());
            m_data.push_back(bs.read_uint8_t());
        }

    public:

        std::vector<uint8_t> m_data;

    };
}
