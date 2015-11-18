// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <sstream>

#include "box.hpp"
#include "../byte_stream.hpp"
#include "../flags.hpp"

namespace petro
{
namespace box
{
    class full_box : public box
    {
    public:

        full_box(const std::string& type, uint32_t size, byte_stream& bs,
            box* parent):
            box(type, size, bs, parent),
            m_version(bs.read_uint8_t()),
            m_flags(bs)
        {
            // 1 for version + 3 for flags
            m_remaining_bytes -= 4;
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << box::describe() << std::endl;
            ss << "  version: " << (uint32_t)m_version;

            return ss.str();
        }

    protected:

        uint8_t m_version;
        flags m_flags;

    };
}
}