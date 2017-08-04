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

#include <iostream>

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

    moov(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    moov(std::weak_ptr<box> parent) :
        box(moov::TYPE, parent)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        box::read(size, bs);
        Parser p;
        auto branched_bs = byte_stream(bs, m_remaining_bytes);
        p.read(branched_bs, shared_from_this());
        assert(branched_bs.remaining_bytes() == 0);
    }

    void parse_box_content(std::error_code& error) override
    {
        assert(!error);
        Parser p;
        p.read3(
            m_bs.remaining_data(),
            m_bs.remaining_size(),
            shared_from_this(), error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }
};

template<class Parser>
const std::string moov<Parser>::TYPE = "moov";
}
}
