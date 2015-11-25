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
    /// movie extends header box
    class mehd : public box
    {

    public:

        static const std::string TYPE;

    public:
        mehd(std::weak_ptr<box> parent):
            box(mehd::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string mehd::TYPE = "mehd";
}
}
