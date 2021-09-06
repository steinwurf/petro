// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

#include "bit_stream.hpp"

namespace petro
{
/// Class holding information about the picture
class picture_parameter_set
{
public:
    /// parse the data
    /// @return shared pointer to a new parameter set
    static std::shared_ptr<picture_parameter_set>
    parse(const uint8_t* data, uint64_t size, std::error_code& error)
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
    /// private constructor
    picture_parameter_set(const uint8_t* data, uint64_t size) : m_bs(data, size)
    {
    }

    /// private parse
    void parse(std::error_code& error)
    {
        assert(!error);
        (void)error;
    }

public:
    /// @return The data of the picture
    const uint8_t* data() const
    {
        return m_bs.data();
    }

    /// @return The size of the data in bytes.
    uint64_t size() const
    {
        return m_bs.size();
    }

    /// @return A descriptive string
    std::string describe() const
    {
        std::stringstream ss;
        ss << "pps:" << std::endl;
        ss << "      size: " << size() << std::endl;

        return ss.str();
    }

private:
    /// bit_stream containing the data
    bit_stream m_bs;
};
}
