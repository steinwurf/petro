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
/// Null media header, overall information (some tracks only)
class nmhd : public full_box
{

public:

    static const std::string TYPE;

public:
    nmhd(std::weak_ptr<box> parent) :
        full_box(nmhd::TYPE, parent)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        full_box::read(size, bs);
        bs.skip(m_remaining_bytes);
    }
};
}
}
