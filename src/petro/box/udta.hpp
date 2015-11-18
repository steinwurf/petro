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
    /// user-data
    template<class Parser>
    class udta : public box
    {

    public:

        static const std::string TYPE;

    public:
        udta(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(udta::TYPE, size, bs, parent)
        {
            Parser p;
            p.read(m_children, bs.data(), m_remaining_bytes, this);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string udta<Parser>::TYPE = "udta";
}
}
