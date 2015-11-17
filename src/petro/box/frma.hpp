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
        frma(byte_stream& bs, uint32_t size):
            box(frma::TYPE)
        {
            bs.skip(size);
        }
    };

    const std::string frma::TYPE = "frma";
}
}
