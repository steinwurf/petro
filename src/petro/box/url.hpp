// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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

    url(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        while (m_bs.remaining_size() != 0)
        {
            uint8_t c;
            m_bs.read(c, error);
            if (error)
                return;
            m_location += c;
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
}
}
