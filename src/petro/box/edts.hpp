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
    /// edit list container
    template<class Parser>
    class edts : public box
    {

    public:

        static const std::string TYPE;

    public:
        edts(byte_stream& parent_bs, uint32_t size):
            box(edts::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string edts<Parser>::TYPE = "edts";
}
}
