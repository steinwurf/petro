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
        trak(std::weak_ptr<box> parent):
            box(trak::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            Parser p;
            auto branched_bs = byte_stream(bs, m_remaining_bytes);
            p.read(shared_from_this(), branched_bs);
        }
    };

    template<class Parser>
    const std::string trak<Parser>::TYPE = "trak";
}
}
