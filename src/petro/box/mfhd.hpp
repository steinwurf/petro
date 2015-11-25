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
    /// movie fragment header
    class mfhd : public box
    {

    public:

        static const std::string TYPE;

    public:
        mfhd(std::weak_ptr<box> parent):
            box(mfhd::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string mfhd::TYPE = "mfhd";
}
}
