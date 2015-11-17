// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// Null media header, overall information (some tracks only)
    class nmhd : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        nmhd(byte_stream& bs, uint32_t size):
            full_box(nmhd::TYPE, bs, size)
        {
            bs.skip(size);
        }
    };

    const std::string nmhd::TYPE = "nmhd";
}
}
