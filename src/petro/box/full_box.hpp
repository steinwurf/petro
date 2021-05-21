// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <memory>
#include <sstream>

#include "data_box.hpp"

namespace petro
{
namespace box
{
class full_box : public data_box
{
public:
    full_box(const uint8_t* data, uint64_t size) : data_box(data, size)
    {
    }

    void parse_box_content(std::error_code& error) override final
    {
        assert(!error);
        m_bs.read<uint8_t>(m_version, error);
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

    std::string box_describe() const override final
    {
        std::stringstream ss;
        ss << "  version: " << (uint32_t)m_version << std::endl;
        ss << full_box_describe();
        return ss.str();
    }

    virtual std::string full_box_describe() const
    {
        return "";
    }

    error box_error_code() const override
    {
        return error::invalid_full_box;
    }

    virtual ~full_box()
    {
    }

protected:
    uint8_t m_version;
    std::vector<uint8_t> m_flags;
};
}
}
