// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"

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

    hdlr(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        assert(!error);
        // predefined
        m_bs.skip(4U, error);
        if (error)
            return;

        m_bs.read_type(m_handler_type, error);
        if (error)
            return;

        // reserved
        m_bs.skip(4U * 3U, error);
        if (error)
            return;

        std::string name = "";
        while (m_bs.remaining_size() != 0)
        {
            uint8_t c = 0;
            m_bs.read(c, error);
            if (error)
                return;

            name += c;
        }
        m_name = name;
    }

    error box_error_code() const override
    {
        return error::invalid_hdlr_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string full_box_describe() const override
    {
        std::stringstream ss;
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
