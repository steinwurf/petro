// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"

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
            uint8_t c = 0;
            m_bs.read<uint8_t>(c, error);
            if (error)
                return;
            m_location += c;
        }
    }

    error box_error_code() const override
    {
        return error::invalid_url_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
        ss << "  location: " << m_location << std::endl;
        return ss.str();
    }

private:

    std::string m_location;
};
}
}
