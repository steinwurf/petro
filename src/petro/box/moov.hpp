// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include "../byte_stream.hpp"
#include "box.hpp"

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
        moov(byte_stream& parent_bs, uint32_t size):
            box(moov::TYPE)
        {
            Parser p;
            m_children = p.read(parent_bs.data(), size);
            parent_bs.skip(size);
        }
    };

    template<class Parser>
    const std::string moov<Parser>::TYPE = "moov";
}
}
