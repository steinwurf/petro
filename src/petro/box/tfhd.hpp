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
    /// track fragment header
    class tfhd : public box
    {

    public:

        static const std::string TYPE;

    public:
        tfhd():
            box(tfhd::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            box::read(size, bs, parent);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string tfhd::TYPE = "tfhd";
}
}
