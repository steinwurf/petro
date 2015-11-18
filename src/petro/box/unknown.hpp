// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
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
        unknown(const std::string& type, uint32_t size, byte_stream& bs, box* parent=nullptr):
            box(type, size, bs, parent)
        {
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << box::describe() << "?" << std::endl;
            return ss.str();
        }
    };
}
}
