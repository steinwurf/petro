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
    /// sample padding bits
    class padb : public box
    {

    public:

        static const std::string TYPE;

    public:
        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            box::read(padb::TYPE, size, bs, parent);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string padb::TYPE = "padb";
}
}
