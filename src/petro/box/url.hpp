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
    url(std::weak_ptr<box> parent) :
        full_box(url::TYPE, parent)
    { }

    void read(uint64_t size, byte_stream& bs)
    {
        full_box::read(size, bs);
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
}
}
