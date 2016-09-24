// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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
    moov(std::weak_ptr<box> parent) :
        box(moov::TYPE, parent)
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
const std::string moov<Parser>::TYPE = "moov";
}
}
