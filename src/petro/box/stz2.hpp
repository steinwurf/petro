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
    /// compact sample sizes (framing)
    class stz2 : public box
    {

    public:

        static const std::string TYPE;

    public:
        stz2(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(stz2::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string stz2::TYPE = "stz2";
}
}
