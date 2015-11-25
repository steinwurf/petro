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
        mdia(std::weak_ptr<box> parent):
            box(mdia::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            Parser p;
            p.read(shared_from_this(), bs.data(), m_remaining_bytes);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string mdia<Parser>::TYPE = "mdia";
}
}

