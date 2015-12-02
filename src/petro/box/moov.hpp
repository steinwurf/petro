// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include "../byte_stream.hpp"
#include "box.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace petro
{
namespace box
{
    /// container for all the metadata
    template<class Parser>
    class moov : public box
    {

    public:

        static const std::string TYPE;

    public:
        moov(std::weak_ptr<box> parent):
            box(moov::TYPE, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            box::read(size, bs);
            Parser p;
            auto branched_bs = byte_stream(bs, m_remaining_bytes);
            p.read(shared_from_this(), branched_bs);
            assert(branched_bs.remaining_bytes() == 0);
        }
    };

    template<class Parser>
    const std::string moov<Parser>::TYPE = "moov";
}
}
