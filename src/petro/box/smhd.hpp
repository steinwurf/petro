// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../helper.hpp"
#include "../byte_stream.hpp"

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

    void read(uint32_t size, byte_stream& bs)
    {
        full_box::read(size, bs);
        m_balance = helper::fixed_point_88(bs.read_uint16_t());
        m_remaining_bytes -= 2;
        bs.skip(2);
        m_remaining_bytes -= 2;
        bs.skip(m_remaining_bytes);
    }

    virtual std::string describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
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
