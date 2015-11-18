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
    /// sync sample table (random access points)
    class stss : public box
    {

    public:

        static const std::string TYPE;

    public:
        stss(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(stss::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string stss::TYPE = "stss";
}
}
