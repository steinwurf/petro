// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include "avcc.hpp"

#include <string>
#include <vector>
#include <memory>
#include "../sequence_parameter_set.hpp"
#include "../picture_parameter_set.hpp"

namespace petro
{
namespace box
{
    const std::string avcc::TYPE = "avcC";

    avcc::avcc(std::weak_ptr<box> parent):
        box(avcc::TYPE, parent)
    { }

    void avcc::read(uint64_t size, byte_stream& bs)
    {
        box::read(size, bs);

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
            std::vector<uint8_t> sps;
            auto sequence_parameter_set_length = bs.read_uint16_t();
            m_remaining_bytes -= 2;
            assert(sequence_parameter_set_length <= m_remaining_bytes);
            for (uint16_t i = 0; i < sequence_parameter_set_length; ++i)
            {
                sps.push_back(bs.read_uint8_t());
                m_remaining_bytes -= 1;
            }
            m_sequence_parameter_sets.push_back(
                std::make_shared<petro::sequence_parameter_set>(sps));
        }

        m_num_of_picture_parameter_sets = bs.read_uint8_t();
        m_remaining_bytes -= 1;

        for (uint8_t i = 0; i < m_num_of_picture_parameter_sets; ++i)
        {
            std::vector<uint8_t> pps;
            auto picture_parameter_set_length = bs.read_uint16_t();
            m_remaining_bytes -= 2;
            assert(picture_parameter_set_length <= m_remaining_bytes);
            for (uint16_t i = 0; i < picture_parameter_set_length; ++i)
            {
                pps.push_back(bs.read_uint8_t());
                m_remaining_bytes -= 1;
            }
            m_picture_parameter_sets.push_back(
                std::make_shared<petro::picture_parameter_set>(pps));
        }

        bs.skip(m_remaining_bytes);
    }

    std::string avcc::describe() const
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
            ss << "    " << sps->describe();
        }

        ss << "  picture_parameter_sets:" << std::endl;
        for (const auto& pps : m_picture_parameter_sets)
        {
            ss << "    " << pps->describe();
        }
        return ss.str();
    }

    std::shared_ptr<petro::sequence_parameter_set> avcc::sequence_parameter_set(
        uint32_t index) const
    {
        assert(index < m_sequence_parameter_sets.size());
        return m_sequence_parameter_sets[index];
    }

    std::shared_ptr<petro::picture_parameter_set> avcc::picture_parameter_set(
        uint32_t index) const
    {
        assert(index < m_picture_parameter_sets.size());
        return m_picture_parameter_sets[index];
    }

    uint8_t avcc::length_size() const
    {
        return m_length_size_minus_one + 1;
    }
}
}
