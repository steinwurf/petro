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
        stbl(byte_stream& parent_bs, uint32_t size):
            box(stbl::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string stbl<Parser>::TYPE = "stbl";
}
}
