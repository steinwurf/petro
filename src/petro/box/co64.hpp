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
    /// 64-bit chunk offset
    class co64 : public box
    {

    public:

        static const std::string TYPE;

    public:
        co64(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(co64::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string co64::TYPE = "co64";
}
}
