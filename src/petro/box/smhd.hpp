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
/// sound media header, overall information (sound track only)
class smhd : public full_box
{

public:

    static const std::string TYPE;

public:
    smhd(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        m_bs.read_fixed_point_88(m_balance, error);
        if (error)
            return;
        m_bs.skip(2, error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
        ss << "  balance: " << m_balance << std::endl;
        return ss.str();
    }

private:

    /// a fixed-point 8.8 number that places mono audio tracks in a stereo
    /// space; 0 is center (the normal value); full left is -1.0 and full
    /// right is 1.0.
    float m_balance;
};
}
}
