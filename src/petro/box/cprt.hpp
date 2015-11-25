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
    /// copyright etc.
    class cprt : public box
    {

    public:

        static const std::string TYPE;

    public:
        cprt(std::weak_ptr<box> parent):
            box(cprt::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string cprt::TYPE = "cprt";
}
}
