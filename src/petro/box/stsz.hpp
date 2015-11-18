// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// sample sizes (framing)
    class stsz : public box
    {

    public:

        static const std::string TYPE;

    public:
        stsz(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(stsz::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string stsz::TYPE = "stsz";
}
}
