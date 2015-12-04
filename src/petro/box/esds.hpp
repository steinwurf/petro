// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// MPEG-4 Elementary Stream Descriptor
    class esds : public box
    {

    public:

        static const std::string TYPE;

    public:
        esds(std::weak_ptr<box> parent);
        void read(uint64_t size, byte_stream& bs);
        std::string describe() const;

    private:

        uint8_t read_section_length(byte_stream& bs);

    private:

        /// A field set to zero.
        uint32_t m_version;

        uint16_t m_es_id;
        uint8_t m_byte_stream_priority;

        std::string m_data;
    };
}
}
