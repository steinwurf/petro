// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// MPEG-4 Elementary Stream Descriptor
    class esds : public full_box
    {

    public:

        class descriptor
        {
        public:
            descriptor(byte_stream& bs, uint8_t tag):
                m_tag(tag)
            {
                // The next byte is either the descriptor length or the first of
                // a three bytes optional extended descriptor type tag string.
                // The tag types are 0x80,0x81,0xFE.
                m_length = bs.read_uint8_t();
                m_size = 1;
                if (m_length == 0x80 ||
                    m_length == 0x81 ||
                    m_length == 0xFE)
                {
                    // appearently we are reading the optional extended
                    // descriptor type tag string - let's throw it away...
                    bs.skip(2);
                    // ... and read the descriptor length.
                    m_length = bs.read_uint8_t();
                    m_size += 3;
                }

                m_size += m_length;
                m_remaining_bytes = m_length;
            }

            uint8_t length()
            {
                return m_length;
            }

            uint32_t size()
            {
                return m_size;
            }

        protected:
            uint8_t m_tag;
            uint8_t m_length;
            uint32_t m_size;
            uint32_t m_remaining_bytes;
        };

        class decoder_specific_info_descriptor : public descriptor
        {
        public:
            decoder_specific_info_descriptor(byte_stream& bs, uint8_t tag):
                descriptor(bs, tag)
            {
                assert(m_tag == 0x05);
                bs.skip(m_remaining_bytes);
            }
        };

        class decoder_config_descriptor : public descriptor
        {
        public:
            decoder_config_descriptor(byte_stream& bs, uint8_t tag):
                descriptor(bs, tag)
            {
                assert(m_tag == 0x04);

                m_object_type_id = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                uint32_t d = bs.read_uint8_t();
                m_remaining_bytes -= 1;
                m_stream_type = d & 0x3F;
                m_up_stream = ((d & 0x40) >> 6) == 1;

                m_buffer_size_db =
                    (int32_t) bs.read_uint8_t() << 16 |
                    (int32_t) bs.read_uint8_t() << 8 |
                    (int32_t) bs.read_uint8_t();
                m_remaining_bytes -= 3;

                m_max_bitrate = bs.read_uint32_t();
                m_remaining_bytes -= 4;

                m_average_bitrate = bs.read_uint32_t();
                m_remaining_bytes -= 4;

                while(m_remaining_bytes != 0)
                {
                    uint8_t nested_descripter_tag = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    m_decoder_specific_info_descriptor =
                        std::make_shared<decoder_specific_info_descriptor>(
                            bs, nested_descripter_tag);
                    m_remaining_bytes -=
                        m_decoder_specific_info_descriptor->size();
                }
            }

        private:
            uint8_t m_object_type_id;
            uint8_t m_stream_type;
            bool m_up_stream;
            uint32_t m_buffer_size_db;
            uint32_t m_max_bitrate;
            uint32_t m_average_bitrate;
            std::shared_ptr<descriptor> m_decoder_specific_info_descriptor;
            // std::vector<std::shared_ptr<descriptor>>
            //     m_profile_level_indication_index_decriptors;
        };


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

        class elemetary_stream_descriptor : public descriptor
        {
        public:
            elemetary_stream_descriptor(byte_stream& bs, uint8_t tag):
                descriptor(bs, tag)
            {
                assert(m_tag == 0x03);
                m_es_id = bs.read_uint16_t();
                m_remaining_bytes -= 2;

                auto d = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                auto stream_dependence_flag = (d & 0x01) == 1;
                auto url_flag = ((d & 0x02) >> 1) == 1;
                auto ocr_stream_flag = ((d & 0x04) >> 2) == 1;

                m_byte_stream_priority = (d & 0xF8) >> 3;

                if (stream_dependence_flag)
                {
                    m_depends_on_esid = bs.read_uint16_t();
                    m_remaining_bytes -= 2;
                }

                if (url_flag)
                {
                    auto url_length = bs.read_uint8_t();
                    m_remaining_bytes -= 1;

                    for (uint8_t i = 0; i < url_length; ++i)
                    {
                        m_url += bs.read_uint8_t();
                        m_remaining_bytes -= 1;
                    }
                }

                if (ocr_stream_flag)
                {
                    m_ocr_es_id = bs.read_uint16_t();
                    m_remaining_bytes -= 2;
                }

                auto decoder_config_descriptor_tag = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                m_decoder_config_descriptor =
                    std::make_shared<decoder_config_descriptor>(
                        bs, decoder_config_descriptor_tag);
                m_remaining_bytes -= m_decoder_config_descriptor->size();

                auto sl_config_descriptor_tag = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                m_sl_config_descriptor =
                    std::make_shared<sl_config_descriptor>(
                        bs, sl_config_descriptor_tag);
                m_remaining_bytes -= m_sl_config_descriptor->size();

                bs.skip(m_remaining_bytes);
            }

        private:

            uint16_t m_es_id;
            uint8_t m_byte_stream_priority;
            uint16_t m_depends_on_esid;
            std::string m_url;
            uint16_t m_ocr_es_id;

            std::shared_ptr<decoder_config_descriptor>
                m_decoder_config_descriptor;

            std::shared_ptr<descriptor> m_sl_config_descriptor;

            // std::shared_ptr<descriptor> m_ipi_decriptor; // 0x09
            // std::vector<std::shared_ptr<descriptor>> m_content_id_descriptor; // 0x07
            // std::vector<std::shared_ptr<descriptor>> m_ipmp_descriptor; // 0x0A
            // std::vector<std::shared_ptr<descriptor>> langDescr; // 0x43
            // std::shared_ptr<descriptor> qosDescr; // 0x0C
            // std::shared_ptr<descriptor> regDescr; // 0x0D
            // std::vector<std::shared_ptr<descriptor>> extDescr; // 0x80
        };

    public:

        static const std::string TYPE;

    public:
        esds(std::weak_ptr<box> parent);
        void read(uint64_t size, byte_stream& bs);
        std::string describe() const;

    private:

        std::shared_ptr<elemetary_stream_descriptor> m_descriptor;
    };
}
}
