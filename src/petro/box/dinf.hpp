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
    /// data information box, container
    template<class Parser>
    class dinf : public box
    {

    public:

        static const std::string TYPE;

    public:
        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            box::read(dinf::TYPE, size, bs, parent);
            Parser p;
            p.read(this, bs.data(), m_remaining_bytes);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string dinf<Parser>::TYPE = "dinf";
}
}
