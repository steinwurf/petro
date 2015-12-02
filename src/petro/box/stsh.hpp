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
    /// shadow sync sample table
    class stsh : public box
    {

    public:

        static const std::string TYPE;

    public:
        stsh(std::weak_ptr<box> parent):
            box(stsh::TYPE, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }
    };
}
}
