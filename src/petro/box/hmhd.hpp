// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
/// hint media header, overall information (hint track only)
class hmhd : public full_box
{

public:

    static const std::string TYPE;

public:
    hmhd(const uint8_t* data, uint64_t size) :
        full_box(data, size)
    { }

    void parse_full_box_content(std::error_code& error) override
    {

        m_bs.read(m_max_pdu_size, error);
        if (error)
            return;

        m_bs.read(m_average_pdu_size, error);
        if (error)
            return;

        m_bs.read(m_max_bit_rate, error);
        if (error)
            return;

        m_bs.read(m_average_bit_rate, error);
        if (error)
            return;

        // reserved
        m_bs.skip(4, error);
        if (error)
            return;

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    virtual std::string describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        ss << "  max_pdu_size: " << m_max_pdu_size << std::endl;
        ss << "  average_pdu_size: " << m_average_pdu_size << std::endl;
        ss << "  max_bit_rate: " << m_max_bit_rate << std::endl;
        ss << "  average_bit_rate: " << m_average_bit_rate << std::endl;
        return ss.str();
    }

private:

    /// the size in bytes of the largest PDU in this (hint) stream
    uint16_t m_max_pdu_size;

    /// the average size of a PDU over the entire presentation
    uint16_t m_average_pdu_size;

    /// the maximum rate in bits/second over any window of one second
    uint32_t m_max_bit_rate;

    /// the average rate in bits/second over the entire presentation
    uint32_t m_average_bit_rate;
};
}
}
