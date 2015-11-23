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
    /// sample table box, container for the time/space map
    template<class Parser>
    class stbl : public box
    {

    public:

        static const std::string TYPE;

    public:
        stbl():
            box(stbl::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            box::read(size, bs, parent);
            Parser p;
            p.read(this, bs.data(), m_remaining_bytes);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string stbl<Parser>::TYPE = "stbl";
}
}
