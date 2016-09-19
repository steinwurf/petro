// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>

#include "descriptor.hpp"

#include "../byte_stream.hpp"

namespace petro
{
    namespace descriptor
    {
        // sync layer config descriptor
        class sl_config_descriptor : public descriptor
        {
        public:
            sl_config_descriptor(byte_stream& bs, uint8_t tag) :
                descriptor(bs, tag)
            {
                assert(m_tag == 0x06);
                auto predefined = bs.read_uint8_t();

                switch (predefined)
                {
                case 0x00:
                {
                    auto d1 = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    m_use_access_unit_start_flag = ((d1 & 0x01) >> 1) == 1;
                    m_use_access_unit_end_flag = ((d1 & 0x02) >> 2) == 1;
                    m_use_random_access_point_flag = ((d1 & 0x04) >> 3) == 1;
                    m_has_random_access_units_only_flag =
                        ((d1 & 0x08) >> 4) == 1;
                    m_use_padding_flag = ((d1 & 0x10) >> 5) == 1;
                    auto use_timestamps_flag = ((d1 & 0x20) >> 6) == 1;
                    m_use_idle_flag = ((d1 & 0x40) >> 7) == 1;
                    auto duration_flag = ((d1 & 0x80) >> 8) == 1;

                    m_time_stamp_resolution = bs.read_uint32_t();
                    m_remaining_bytes -= 4;

                    m_ocr_resolution = bs.read_uint32_t();
                    m_remaining_bytes -= 4;

                    // must be less than 64
                    auto time_stamp_length = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    // must be less than 64
                    m_ocr_length = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    // must be less than 32
                    m_au_length = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    m_instant_bitrate_length = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    auto d2 = bs.read_uint16_t();
                    m_remaining_bytes -= 2;
                    m_degradation_priority_length = d2;
                    m_au_sequence_number_length = d2;
                    m_packet_sequence_number_length = d2;

                    if (duration_flag)
                    {
                        m_time_scale = bs.read_uint32_t();
                        m_remaining_bytes -= 4;

                        m_access_unit_duration = bs.read_uint16_t();
                        m_remaining_bytes -= 2;

                        m_composition_unit_duration = bs.read_uint16_t();
                        m_remaining_bytes -= 2;
                    }

                    if (!use_timestamps_flag)
                    {
                        auto time_stamp_length_bytes = time_stamp_length / 8;
                        if (time_stamp_length % 8 != 0)
                            time_stamp_length_bytes += 1;

                        /// @todo read the next time_stamp_length_bytes
                        // m_start_decoding_time_stamp =
                        //     bs.read_bits(time_stamp_length_bytes);
                        bs.skip(time_stamp_length_bytes);
                        m_remaining_bytes -= time_stamp_length_bytes;

                        /// @todo read the next time_stamp_length_bytes
                        // m_start_composition_time_stamp =
                        //     bs.read_bits(time_stamp_length_bytes);
                        bs.skip(time_stamp_length_bytes);
                        m_remaining_bytes -= time_stamp_length_bytes;
                    }
                    break;
                }
                default:
                {
                    // unsupported sync layer descriptor predefine
                }
                }
            }


            bool use_access_unit_start_flag() const
            {
                return m_use_access_unit_start_flag;
            }

            bool use_access_unit_end_flag() const
            {
                return m_use_access_unit_end_flag;
            }

            bool use_random_access_point_flag() const
            {
                return m_use_random_access_point_flag;
            }

            bool has_random_access_units_only_flag() const
            {
                return m_has_random_access_units_only_flag;
            }

            bool use_padding_flag() const
            {
                return m_use_padding_flag;
            }

            bool use_idle_flag() const
            {
                return m_use_idle_flag;
            }

            bool duration_flag() const
            {
                return m_duration_flag;
            }

            uint32_t time_stamp_resolution() const
            {
                return m_time_stamp_resolution;
            }

            uint32_t ocr_resolution() const
            {
                return m_ocr_resolution;
            }

            uint8_t ocr_length() const
            {
                return m_ocr_length;
            }

            uint8_t au_length() const
            {
                return m_au_length;
            }

            uint8_t instant_bitrate_length() const
            {
                return m_instant_bitrate_length;
            }

            uint8_t degradation_priority_length() const
            {
                return m_degradation_priority_length;
            }

            uint8_t au_sequence_number_length() const
            {
                return m_au_sequence_number_length;
            }

            uint8_t packet_sequence_number_length() const
            {
                return m_packet_sequence_number_length;
            }

            uint32_t time_scale() const
            {
                return m_time_scale;
            }

            uint16_t access_unit_duration() const
            {
                return m_access_unit_duration;
            }

            uint16_t composition_unit_duration() const
            {
                return m_composition_unit_duration;
            }

        private:

            bool m_use_access_unit_start_flag;
            bool m_use_access_unit_end_flag;
            bool m_use_random_access_point_flag;
            bool m_has_random_access_units_only_flag;
            bool m_use_padding_flag;
            bool m_use_idle_flag;
            bool m_duration_flag;


            uint32_t m_time_stamp_resolution;
            uint32_t m_ocr_resolution;
            uint8_t m_ocr_length; // must be less than 64
            uint8_t m_au_length; // must be less than 32
            uint8_t m_instant_bitrate_length;
            uint8_t m_degradation_priority_length; //bit 4
            uint8_t m_au_sequence_number_length; //bit 5
            uint8_t m_packet_sequence_number_length; //bit 5

            uint32_t m_time_scale;
            uint16_t m_access_unit_duration;
            uint16_t m_composition_unit_duration;

            // uint64_t m_start_decoding_time_stamp; // bit(timeStampLength)
            // uint64_t m_start_composition_time_stamp; // bit(timeStampLength)
        };
    }
}
