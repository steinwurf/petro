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
    /// primary item reference
    class pitm : public box
    {

    public:

        static const std::string TYPE;

    public:
        pitm(std::weak_ptr<box> parent):
            box(pitm::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string pitm::TYPE = "pitm";
}
}
