// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "../stream.hpp"
#include "tag.hpp"

namespace petro
{
namespace descriptor
{
class descriptor
{
public:

    descriptor(const uint8_t* data, uint64_t size) :
        m_bs(data, size)
    { }

    void parse(std::error_code& error)
    {
        assert(!error);
        m_bs.seek(0, error);
        if (error)
            return;

        m_bs.read(m_tag, error);
        if (error)
            return;

        // The next byte is either the descriptor length or the first of
        // a three bytes optional extended descriptor type tag string.
        // The tag types are 0x80,0x81,0xFE.
        uint8_t length = 0;
        m_bs.read(length, error);

        if (length == 0x80 ||
            length == 0x81 ||
            length == 0xFE)
        {
            // appearently we are reading the optional extended
            // descriptor type tag string - let's throw it away...
            m_bs.skip(2, error);
            if (error)
                return;

            // ... and read the descriptor length.
            m_bs.read(length, error);
            if (error)
                return;
        }

        // the length variable defines the length of the descriptor,
        // excluding the tag, optional extended descriptor type tag, and length.
        // hence the total size is how much we've read already, plus the legnth.
        auto position = m_bs.position();
        auto total_size = position + length;
        m_bs = stream(m_bs.data(), total_size);
        m_bs.seek(position, error);
        if (error)
            return;
        parse_descriptor_content(error);
    }

    virtual void parse_descriptor_content(std::error_code& error)
    {
        assert(!error);
        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    /// The total size in bytes used by this descriptor
    uint32_t size()
    {
        return m_bs.size();
    }

protected:

    stream m_bs;
    tag m_tag = tag(0U);
};
}
}
