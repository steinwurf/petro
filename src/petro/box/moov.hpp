// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_box.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace petro
{
namespace box
{
/// container for all the metadata
template<class Parser>
class moov : public data_box
{

public:

    static const std::string TYPE;

public:

    moov(const uint8_t* data, uint64_t size) :
        data_box(data, size)
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
        return error::invalid_moov_box;
    }

    std::string type() const override
    {
        return TYPE;
    }
};

template<class Parser>
const std::string moov<Parser>::TYPE = "moov";
}
}
