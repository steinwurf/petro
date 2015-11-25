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
    /// original format box
    class frma : public box
    {

    public:

        static const std::string TYPE;

    public:
        frma(std::weak_ptr<box> parent):
            box(frma::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }
    };

    const std::string frma::TYPE = "frma";
}
}
