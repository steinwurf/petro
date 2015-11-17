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
        url(byte_stream& bs, uint32_t size):
            full_box(url::TYPE, bs, size)
        {
            while (size != 0)
            {
                m_location += bs.read_uint8_t();
                size -= 1;
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
