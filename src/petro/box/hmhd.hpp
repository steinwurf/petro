// Copyright Steinwurf ApS 2015.
// All Rights Reserved

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
        hmhd():
            full_box(hmhd::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            full_box::read(size, bs, parent);
            m_max_pdu_size = bs.read_uint16_t();
            m_remaining_bytes -= 2;

            m_average_pdu_size = bs.read_uint16_t();
            m_remaining_bytes -= 2;

            m_max_bit_rate = bs.read_uint32_t();
            m_remaining_bytes -= 4;

            m_average_bit_rate = bs.read_uint32_t();
            m_remaining_bytes -= 4;

            // reserved
            bs.skip(4);
            m_remaining_bytes -= 4;

            bs.skip(m_remaining_bytes);
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

    const std::string hmhd::TYPE = "hmhd";
}
}
