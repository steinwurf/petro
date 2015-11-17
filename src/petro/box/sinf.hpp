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
    /// protection scheme information box
    template<class Parser>
    class sinf : public box
    {

    public:

        static const std::string TYPE;

    public:
        sinf(byte_stream& parent_bs, uint32_t size):
            box(sinf::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string sinf<Parser>::TYPE = "sinf";
}
}
