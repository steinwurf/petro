// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "descriptor.hpp"
#include "decoder_specific_info_descriptor.hpp"
#include "empty_descriptor.hpp"

#include "../byte_stream.hpp"

namespace petro
{
namespace descriptor
{
    class decoder_config_descriptor : public descriptor
    {

    public:

        using decoder_specific_info_descriptor_type =
            petro::descriptor::decoder_specific_info_descriptor;

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

            if (m_remaining_bytes == 0)
                return;

            uint8_t nested_descripter_tag = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            // the decoder specific info descriptor is optional
            if (nested_descripter_tag == 0x05)
            {
                m_decoder_specific_info_descriptor =
                    std::make_shared<decoder_specific_info_descriptor_type>(
                        bs, nested_descripter_tag);
                m_remaining_bytes -=
                    m_decoder_specific_info_descriptor->size();

                // if we still have more bytes left, we need to get the new
                // tag.
                if (m_remaining_bytes != 0)
                {
                    nested_descripter_tag = bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }
                else
                {
                    return;
                }
            }

            // now 0-many profile level indication index descriptor
            do
            {
                auto profile_level_indication_index_descriptor =
                    std::make_shared<empty_descriptor>(
                        bs, nested_descripter_tag);
                m_remaining_bytes -=
                    profile_level_indication_index_descriptor->size();

                m_profile_level_indication_index_descriptors.push_back(
                    profile_level_indication_index_descriptor
                );

                if (m_remaining_bytes != 0)
                {
                    nested_descripter_tag = bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }

            } while (m_remaining_bytes != 0);
        }

        uint8_t object_type_id() const
        {
            return m_object_type_id;
        }

        std::shared_ptr<decoder_specific_info_descriptor_type>
            decoder_specific_info_descriptor() const
        {
            return m_decoder_specific_info_descriptor;
        }

    private:
        uint8_t m_object_type_id;
        uint8_t m_stream_type;
        bool m_up_stream;
        uint32_t m_buffer_size_db;
        uint32_t m_max_bitrate;
        uint32_t m_average_bitrate;
        std::shared_ptr<decoder_specific_info_descriptor_type>
            m_decoder_specific_info_descriptor;
        std::vector<std::shared_ptr<descriptor>>
            m_profile_level_indication_index_descriptors;
    };
}
}
