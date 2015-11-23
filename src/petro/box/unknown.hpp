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
        void read(const std::string& type, uint32_t size, byte_stream& bs,
            box* parent=nullptr)
        {
            box::read(type + "?", size, bs, parent);
            bs.skip(m_remaining_bytes);
        }
    };
}
}
