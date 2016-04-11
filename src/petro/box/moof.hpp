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
    /// movie fragment
    template<class Parser>
    class moof : public box
    {

    public:

        static const std::string TYPE;

    public:
        moof(std::weak_ptr<box> parent):
            box(moof::TYPE, parent)
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
    const std::string moof<Parser>::TYPE = "moof";
}
}
