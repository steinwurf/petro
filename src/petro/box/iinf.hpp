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
    /// item information
    class iinf : public box
    {

    public:

        static const std::string TYPE;

    public:
        iinf(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(iinf::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string iinf::TYPE = "iinf";
}
}
