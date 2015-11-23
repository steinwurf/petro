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
        moov():
            box(moov::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            box::read(size, bs, parent);
            Parser p;
            p.read(this, bs.data(), m_remaining_bytes);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string moov<Parser>::TYPE = "moov";
}
}
