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
    dref(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {
        assert(!error);
        m_bs.read(m_entry_count, error);
        if (error)
            return;
        Parser p;
        for (uint32_t i = 0; i < m_entry_count; ++i)
        {
            auto box = p.parse_box(
                m_bs.remaining_data(),
                m_bs.remaining_size(),
                shared_from_this(),
                error);

            if (error)
                return;

            m_bs.skip(box->size(), error);
            if (error)
                return;
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
