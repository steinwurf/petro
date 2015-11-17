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
    /// free space
    template<class Parser>
    class skip : public box
    {

    public:

        static const std::string TYPE;

    public:
        skip(byte_stream& parent_bs, uint32_t size):
            box(skip::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string skip<Parser>::TYPE = "skip";
}
}
