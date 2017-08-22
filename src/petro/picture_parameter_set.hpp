// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <sstream>
#include <vector>
#include <cassert>
#include <memory>

#include "bit_stream.hpp"

namespace petro
{
class picture_parameter_set
{
public:

    static std::shared_ptr<picture_parameter_set> parse(
        const uint8_t* data, uint64_t size, std::error_code& error)
    {
        std::shared_ptr<picture_parameter_set> pps(
            new picture_parameter_set(data, size));
        pps->parse(error);
        if (error)
            return nullptr;
        else
            return pps;
    }

private:

    picture_parameter_set(const uint8_t* data, uint32_t size) :
        m_bs(data, size)
    { }

    void parse(std::error_code& error)
    {
        assert(!error);
    }

public:

    const uint8_t* data() const
    {
        return m_bs.data();
    }

    uint64_t size() const
    {
        return m_bs.size();
    }

    std::string describe() const
    {
        std::stringstream ss;
        ss << "pps:" << std::endl;
        ss << "      size: " << size() << std::endl;

        return ss.str();
    }

private:

    bit_stream m_bs;
};
}
