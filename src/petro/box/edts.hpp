// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
#include <cassert>

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
        edts(std::weak_ptr<box> parent):
            box(edts::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            Parser p;
            auto branched_bs = byte_stream(bs, m_remaining_bytes);
            p.read(shared_from_this(), branched_bs);
            assert(branched_bs.remaining_bytes() == 0);
        }
    };

    template<class Parser>
    const std::string edts<Parser>::TYPE = "edts";
}
}
