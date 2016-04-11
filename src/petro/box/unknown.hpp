// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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

        unknown(const std::string& type, std::weak_ptr<box> parent):
            box(type, parent)
        { }

        void read(uint64_t size, byte_stream& bs)
        {
            box::read(size, bs);
            bs.skip(m_remaining_bytes);
        }

        virtual std::string type() const
        {
            return m_type + "?";
        }
    };
}
}
