// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// MPEG-4 decoder configuration
    class avcc : public box
    {

    public:

        struct sequence_parameter_set
        {
            uint16_t sequence_parameter_set_length;
            std::vector<uint8_t> sequence_parameter_set_nal_unit;
        };

        struct picture_parameter_set
        {
            uint16_t picture_parameter_set_length;
            std::vector<uint8_t> picture_parameter_set_nal_unit;
        };

    public:

        static const std::string TYPE;

    public:
        avcc():
            box(avcc::TYPE)
        { }

        void read(uint32_t size, byte_stream& bs, box* parent)
        {
            box::read(size, bs, parent);

            m_configuration_version = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            m_avc_profile_indication = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            m_profile_compatibility = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            m_avc_level_indication = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            m_length_size_minus_one = bs.read_uint8_t() & 0x03;
            m_remaining_bytes -= 1;

            m_num_of_sequence_parameter_sets = bs.read_uint8_t() & 0x1F;
            m_remaining_bytes -= 1;

            for (uint8_t i = 0; i < m_num_of_sequence_parameter_sets; ++i)
            {
                sequence_parameter_set sps;
                sps.sequence_parameter_set_length = bs.read_uint16_t();
                m_remaining_bytes -= 2;
                assert(sps.sequence_parameter_set_length <= m_remaining_bytes);
                for (uint16_t i = 0; i < sps.sequence_parameter_set_length; ++i)
                {
                    sps.sequence_parameter_set_nal_unit.push_back(
                        bs.read_uint8_t());
                    m_remaining_bytes -= 1;
                }
                m_sequence_parameter_sets.push_back(sps);
            }

            m_num_of_picture_parameter_sets = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            for (uint8_t i = 0; i < m_num_of_picture_parameter_sets; ++i)
            {
                picture_parameter_set pps;
                pps.picture_parameter_set_length = bs.read_uint16_t();
                m_remaining_bytes -= 2;
                assert(pps.picture_parameter_set_length <= m_remaining_bytes);
                for (uint16_t i = 0; i < pps.picture_parameter_set_length; ++i)
                {
                    pps.picture_parameter_set_nal_unit.push_back(
                        bs.read_uint8_t());
                    m_remaining_bytes -= 1;
                }
                m_picture_parameter_sets.push_back(pps);
            }

            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << box::describe() << std::endl;
            ss << "  configuration_version: " << (uint32_t)m_configuration_version << std::endl;
            ss << "  avc_profile_indication: " << (uint32_t)m_avc_profile_indication << std::endl;
            ss << "  profile_compatibility: " << (uint32_t)m_profile_compatibility << std::endl;
            ss << "  avc_level_indication: " << (uint32_t)m_avc_level_indication << std::endl;
            ss << "  length_size_minus_one: " << (uint32_t)m_length_size_minus_one << std::endl;
            ss << "  num_of_sequence_parameter_sets: " << (uint32_t)m_num_of_sequence_parameter_sets << std::endl;

            ss << "  sequence_parameter_sets:" << std::endl;
            for (const auto& sps : m_sequence_parameter_sets)
            {
                ss << "     sequence_parameter_set_length: " << (uint32_t)sps.sequence_parameter_set_length << std::endl;
            }

            ss << "  picture_parameter_sets:" << std::endl;
            for (const auto& pps : m_picture_parameter_sets)
            {
                ss << "     picture_parameter_set_length: " << (uint32_t)pps.picture_parameter_set_length << std::endl;
            }
            return ss.str();
        }

    private:

        uint8_t m_configuration_version;
        uint8_t m_avc_profile_indication;
        uint8_t m_profile_compatibility;
        uint8_t m_avc_level_indication;
        uint8_t m_length_size_minus_one;
        uint8_t m_num_of_sequence_parameter_sets;
        std::vector<sequence_parameter_set> m_sequence_parameter_sets;
        uint8_t m_num_of_picture_parameter_sets;
        std::vector<picture_parameter_set> m_picture_parameter_sets;

    };
    const std::string avcc::TYPE = "avcC";
}
}
