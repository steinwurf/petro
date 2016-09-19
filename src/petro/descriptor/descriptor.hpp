// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "../byte_stream.hpp"

namespace petro
{
    namespace descriptor
    {
        class descriptor
        {
        public:
            descriptor(byte_stream& bs, uint8_t tag) :
                m_tag(tag)
            {
                // The next byte is either the descriptor length or the first of
                // a three bytes optional extended descriptor type tag string.
                // The tag types are 0x80,0x81,0xFE.
                auto length = bs.read_uint8_t();
                m_size = 1;
                if (length == 0x80 ||
                    length == 0x81 ||
                    length == 0xFE)
                {
                    // appearently we are reading the optional extended
                    // descriptor type tag string - let's throw it away...
                    bs.skip(2);
                    // ... and read the descriptor length.
                    length = bs.read_uint8_t();
                    m_size += 3;
                }

                // to know how much we have (will) read from the byte stream in
                // this descriptor object, we increment the size member
                // variable m_size.
                m_size += length;

                // for descriptors the bytes to be read is the length, not
                // including the bytes for reading the length or the tag.
                m_remaining_bytes = length;
            }

            /// The total size in bytes used by this descriptor
            uint32_t size()
            {
                return m_size;
            }

        protected:
            uint8_t m_tag;
            uint32_t m_size;
            uint32_t m_remaining_bytes;
        };
    }
}
