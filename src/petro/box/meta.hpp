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
    /// metadata
    template<class Parser>
    class meta : public box
    {

    public:

        static const std::string TYPE;

    public:
        meta(byte_stream& parent_bs, uint32_t size):
            box(meta::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string meta<Parser>::TYPE = "meta";
}
}
