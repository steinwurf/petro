// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cassert>
#include <cstdint>

#include "descriptor.hpp"

#include "../byte_stream.hpp"

namespace petro
{
namespace descriptor
{
    class sl_config_descriptor : public descriptor
    {
    public:
        sl_config_descriptor(byte_stream& bs, uint8_t tag):
            descriptor(bs, tag)
        {
            assert(m_tag == 0x06);
            auto predefined = bs.read_uint8_t();
            if (predefined == 0)
            {
                auto d1 = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                m_use_access_unit_start_flag = ((d1 & 0x01) >> 1) == 1;
                m_use_access_unit_end_flag = ((d1 & 0x02) >> 2) == 1;
                m_use_random_access_point_flag = ((d1 & 0x04) >> 3) == 1;
                m_has_random_access_units_only_flag = ((d1 & 0x08) >> 4) == 1;
                m_use_padding_flag = ((d1 & 0x10) >> 5) == 1;
                m_use_time_stamps_flag = ((d1 & 0x20) >> 6) == 1;
                m_use_idle_flag = ((d1 & 0x40) >> 7) == 1;
                m_duration_flag = ((d1 & 0x80) >> 8) == 1;

                m_time_stamp_resolution = bs.read_uint32_t();
                m_remaining_bytes -= 4;

                m_ocr_resolution = bs.read_uint32_t();
                m_remaining_bytes -= 4;

                m_time_stamp_length = bs.read_uint8_t(); // must be less than 64
                m_remaining_bytes -= 1;

                m_ocr_length = bs.read_uint8_t(); // must be less than 64
                m_remaining_bytes -= 1;

                m_au_length = bs.read_uint8_t(); // must be less than 32
                m_remaining_bytes -= 1;

                m_instant_bitrate_length = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                auto d2 = bs.read_uint16_t();
                m_remaining_bytes -= 2;
                m_degradation_priority_length = d2;
                m_au_sequence_number_length = d2;
                m_packet_sequence_number_length = d2;

                if (m_duration_flag)
                {
                    m_time_scale = bs.read_uint32_t();
                    m_remaining_bytes -= 4;

                    m_access_unit_duration = bs.read_uint16_t();
                    m_remaining_bytes -= 2;

                    m_composition_unit_duration = bs.read_uint16_t();
                    m_remaining_bytes -= 2;
                }

                if (!m_use_time_stamps_flag)
                {
                    auto time_stamp_length_bytes = m_time_stamp_length / 8;
                    if (m_time_stamp_length % 8 != 0)
                        time_stamp_length_bytes += 1;

                    //m_start_decoding_time_stamp = bs.read_bits(time_stamp_length_bytes);
                    bs.skip(time_stamp_length_bytes);
                    m_remaining_bytes -= time_stamp_length_bytes;

                    //m_start_composition_time_stamp = bs.read_bits(time_stamp_length_bytes);
                    bs.skip(time_stamp_length_bytes);
                    m_remaining_bytes -= time_stamp_length_bytes;
                }
            }
        }

    private:
            bool m_use_access_unit_start_flag;
            bool m_use_access_unit_end_flag;
            bool m_use_random_access_point_flag;
            bool m_has_random_access_units_only_flag;
            bool m_use_padding_flag;
            bool m_use_time_stamps_flag;
            bool m_use_idle_flag;
            bool m_duration_flag;


            uint32_t m_time_stamp_resolution;
            uint32_t m_ocr_resolution;
            uint8_t m_time_stamp_length; // must be less than 64
            uint8_t m_ocr_length; // must be less than 64
            uint8_t m_au_length; // must be less than 32
            uint8_t m_instant_bitrate_length;
            uint8_t m_degradation_priority_length; //bit 4
            uint8_t m_au_sequence_number_length; //bit 5
            uint8_t m_packet_sequence_number_length; //bit 5

            uint32_t m_time_scale;
            uint16_t m_access_unit_duration;
            uint16_t m_composition_unit_duration;

            uint64_t m_start_decoding_time_stamp; // bit(timeStampLength)
            uint64_t m_start_composition_time_stamp; // bit(timeStampLength)
    };
}
}
