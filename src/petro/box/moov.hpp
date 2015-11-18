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
        moov(uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(moov::TYPE, size, bs, parent)
        {
            Parser p;
            p.read(m_children, bs.data(), m_remaining_bytes, this);
            bs.skip(m_remaining_bytes);
        }
    };

    template<class Parser>
    const std::string moov<Parser>::TYPE = "moov";
}
}
