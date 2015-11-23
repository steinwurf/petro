// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// url
    class url : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        url(uint32_t size, byte_stream& bs, box* parent=nullptr):
            full_box(url::TYPE, size, bs, parent)
        {
            while (m_remaining_bytes != 0)
            {
                m_location += bs.read_uint8_t();
                m_remaining_bytes -= 1;
            }
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  location: " << m_location << std::endl;
            return ss.str();
        }

    private:

        std::string m_location;
    };

    const std::string url::TYPE = "url ";
}
}