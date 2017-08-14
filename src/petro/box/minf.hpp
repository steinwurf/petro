// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <cassert>

#include "box.hpp"

namespace petro
{
namespace box
{
/// media information container
template<class Parser>
class minf : public box
{

public:

    static const std::string TYPE;

public:

    minf(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    void parse_box_content(std::error_code& error) override
    {
        assert(!error);
        Parser p;
        p.parse(
            m_bs.remaining_data(),
            m_bs.remaining_size(),
            shared_from_this(), error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    error box_error_code() const override
    {
        return error::invalid_minf_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};

template<class Parser>
const std::string minf<Parser>::TYPE = "minf";
}
}
