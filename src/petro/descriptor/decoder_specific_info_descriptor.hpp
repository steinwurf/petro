// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>

#include "descriptor.hpp"

#include "../byte_stream.hpp"

namespace petro
{
namespace descriptor
{
    class decoder_specific_info_descriptor : public descriptor
    {
    public:
        decoder_specific_info_descriptor(byte_stream& bs, uint8_t tag):
            descriptor(bs, tag)
        {
            assert(m_tag == 0x05);
            while (m_remaining_bytes != 0)
            {
                m_specific_info.push_back(bs.read_uint8_t());
                m_remaining_bytes -= 1;
            }
        }


        std::vector<uint8_t> specific_info() const
        {
            return m_specific_info;
        }

    private:

        std::vector<uint8_t> m_specific_info;
    };
}
}
