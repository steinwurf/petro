// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "url.hpp"
#include "urn.hpp"
#include "../byte_stream.hpp"

namespace petro
{
    namespace box
    {
        /// data reference box, declares source(s) of media data in track
        template<class Parser>
        class dref : public full_box
        {
        public:

            static const std::string TYPE;

        public:
            dref(std::weak_ptr<box> parent) :
                full_box(dref::TYPE, parent)
            { }

            void read(uint64_t size, byte_stream& bs)
            {
                full_box::read(size, bs);
                m_entry_count = bs.read_uint32_t();
                m_remaining_bytes -= 4;
                Parser p;
                for (uint32_t i = 0; i < m_entry_count; ++i)
                {
                    p.parse(bs, shared_from_this());
                }
            }

            virtual std::string describe() const
            {
                std::stringstream ss;
                ss << full_box::describe() << std::endl;
                ss << "  entry_count: " << m_entry_count << std::endl;
                return ss.str();
            }

        private:

            /// an integer that counts the actual entries
            uint32_t m_entry_count;
        };

        template<class Parser>
        const std::string dref<Parser>::TYPE = "dref";
    }
}
