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
    /// chunk offset, partial data-offset information
    class stco : public box
    {

    public:

        static const std::string TYPE;

    public:
        stco(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(stco::TYPE, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string stco::TYPE = "stco";
}
}
