// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <sstream>
#include <cassert>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// unknown box
    class unknown : public box
    {
    public:

        unknown(const std::string& type):
            box(type + "?")
        { }

        void read(uint32_t size, byte_stream& bs,
            box* parent=nullptr)
        {
            box::read(size, bs, parent);
            bs.skip(m_remaining_bytes);
        }
    };
}
}
