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
    /// container for the media information in a track
    template<class Parser>
    class mdia : public box
    {

    public:

        static const std::string TYPE;

    public:
        mdia(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(mdia::TYPE, size, bs, parent)
        {
            Parser p;
            p.read(m_children, bs.data(), m_remaining_bytes, this);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string mdia<Parser>::TYPE = "mdia";
}
}

