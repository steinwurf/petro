// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <sstream>
#include <vector>

namespace petro
{
class picture_parameter_set
{
public:
    picture_parameter_set(const uint8_t* data, uint32_t size) :
        m_data(data),
        m_size(size)
    { }

    const uint8_t* data() const
    {
        return m_data;
    }

    uint32_t size() const
    {
        return m_size;
    }

    std::string describe() const
    {
        std::stringstream ss;
        ss << "pps:" << std::endl;
        ss << "      size: " << size() << std::endl;

        return ss.str();
    }

private:

    const uint8_t* m_data;
    uint32_t m_size;
};
}
