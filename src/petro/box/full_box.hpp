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

namespace petro
{
namespace box
{
class full_box : public box
{
public:

    full_box(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    void read(uint32_t size, byte_stream& bs)
    {
        box::read(size, bs);
        m_version = bs.read_uint8_t();
        m_remaining_bytes -= 1;
        // read 24 bytes
        m_flags.push_back(bs.read_uint8_t());
        m_flags.push_back(bs.read_uint8_t());
        m_flags.push_back(bs.read_uint8_t());
        m_remaining_bytes -= 3;
    }

    void parse_box_content(std::error_code& error) override final
    {
        assert(!error);
        m_bs.read(m_version, error);
        if (error)
            return;

        // read 24 bytes
        m_flags.resize(3);
        m_bs.read(m_flags.data(), m_flags.size(), error);
        if (error)
            return;

        parse_full_box_content(error);
        if (error)
            return;
    }

    virtual void parse_full_box_content(std::error_code& error)
    {
        assert(!error);
        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
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
    std::vector<uint8_t> m_flags;
};
}
}
