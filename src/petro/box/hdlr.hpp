// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
    namespace box
    {
        /// handler, declares the media (handler) type
        class hdlr : public full_box
        {

        public:

            static const std::string TYPE;

        public:
            hdlr(std::weak_ptr<box> parent) :
                full_box(hdlr::TYPE, parent)
            { }

            void read(uint64_t size, byte_stream& bs)
            {
                full_box::read(size, bs);
                // predefined
                bs.skip(4);
                m_remaining_bytes -= 4;

                m_handler_type = bs.read_type();
                m_remaining_bytes -= 4;

                // reserved
                bs.skip(4 * 3);
                m_remaining_bytes -= 4 * 3;

                while (m_remaining_bytes != 0)
                {
                    m_name += bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }
            }

            virtual std::string describe() const
            {
                std::stringstream ss;
                ss << full_box::describe() << std::endl;
                ss << "  handler_type: " << m_handler_type << std::endl;
                ss << "  name: " << m_name << std::endl;
                return ss.str();
            }

            std::string handler_type() const
            {
                return m_handler_type;
            }

        private:

            /// when present in a media box, is an integer containing one of the
            /// following values, or a value from a derived specification:
            ///     "vide": Video track
            ///     "soun": Audio track
            ///     "hint": Hint track
            ///
            /// when present in a meta box, contains an appropriate value to
            /// indicate the format of the meta box contents
            std::string m_handler_type;

            /// a null-terminated string in UTF-8 characters which gives a
            /// human-readable name for the track type
            /// (for debugging and inspection purposes).
            std::string m_name;
        };
    }
}
