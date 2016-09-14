// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
    /// track fragment
    template<class Parser>
    class traf : public box
    {

    public:

        static const std::string TYPE;

    public:
        traf(std::weak_ptr<box> parent):
            box(traf::TYPE, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            box::read(size, bs);
            Parser p;
            auto branched_bs = byte_stream(bs, m_remaining_bytes);
            p.read(branched_bs, shared_from_this());
            assert(branched_bs.remaining_bytes() == 0);
        }
    };

    template<class Parser>
    const std::string traf<Parser>::TYPE = "traf";
}
}
