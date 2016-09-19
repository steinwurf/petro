// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <sstream>
#include <memory>

#include "box.hpp"
#include "../byte_stream.hpp"
#include "../flags.hpp"

namespace petro
{
    namespace box
    {
        class full_box : public box
        {
        public:

            full_box(const std::string& type, std::weak_ptr<box> parent) :
                box(type, parent)
            { }

            void read(uint64_t size, byte_stream& bs)
            {
                box::read(size, bs);
                m_version = bs.read_uint8_t();
                m_remaining_bytes -= 1;
                m_flags.read(bs);
                m_remaining_bytes -= 3;
            }

            virtual std::string describe() const
            {
                std::stringstream ss;
                ss << box::describe() << std::endl;
                ss << "  version: " << (uint32_t)m_version;

                return ss.str();
            }

        protected:

            uint8_t m_version;
            flags m_flags;

        };
    }
}
