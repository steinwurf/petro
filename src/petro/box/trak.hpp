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
    /// container for an individual track or stream
    template<class Parser>
    class trak : public box
    {

    public:

        static const std::string TYPE;

    public:
        trak(byte_stream& parent_bs, uint32_t size):
            box(trak::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string trak<Parser>::TYPE = "trak";
}
}
