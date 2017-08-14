// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <cassert>
#include <vector>
#include <memory>

#include "box.hpp"
#include "../sequence_parameter_set.hpp"
#include "../picture_parameter_set.hpp"

namespace petro
{
namespace box
{
/// MPEG-4 decoder configuration
class avcc : public box
{
public:

    static const std::string TYPE;

public:

    avcc(const uint8_t* data, uint64_t size) :
        box(data, size)
    { }

    void parse_box_content(std::error_code& error) override
    {
        assert(!error);
        m_bs.read(m_configuration_version, error);
        if (error)
            return;

        m_bs.read(m_avc_profile_indication, error);
        if (error)
            return;

        m_bs.read(m_profile_compatibility, error);
        if (error)
            return;

        m_bs.read(m_avc_level_indication, error);
        if (error)
            return;

        uint8_t length_size_minus_one_value = 0;
        m_bs.read(length_size_minus_one_value, error);
        if (error)
            return;
        m_length_size_minus_one = length_size_minus_one_value & 0x03;

        uint8_t num_of_sequence_parameter_sets_value = 0;
        m_bs.read(num_of_sequence_parameter_sets_value, error);
        if (error)
            return;
        m_num_of_sequence_parameter_sets = num_of_sequence_parameter_sets_value & 0x1F;

        for (uint8_t i = 0; i < m_num_of_sequence_parameter_sets; ++i)
        {
            uint16_t sequence_parameter_set_length = 0;
            m_bs.read(sequence_parameter_set_length, error);
            if (error)
                return;

            auto sequence_parameter_set_data = m_bs.remaining_data();
            m_bs.skip(sequence_parameter_set_length, error);
            if (error)
                return;
            auto sequence_parameter_set =
                std::make_shared<petro::sequence_parameter_set>(
                    sequence_parameter_set_data,
                    sequence_parameter_set_length);

            sequence_parameter_set->parse(error);
            if (error)
                return;

            m_sequence_parameter_sets.push_back(sequence_parameter_set);
        }

        m_bs.read(m_num_of_picture_parameter_sets, error);
        if (error)
            return;

        for (uint8_t i = 0; i < m_num_of_picture_parameter_sets; ++i)
        {

            uint16_t picture_parameter_set_length = 0;
            m_bs.read(picture_parameter_set_length, error);
            if (error)
                return;

            auto picture_parameter_set_data = m_bs.remaining_data();
            m_bs.skip(picture_parameter_set_length, error);
            if (error)
                return;
            auto picture_parameter_set =
                std::make_shared<petro::picture_parameter_set>(
                    picture_parameter_set_data,
                    picture_parameter_set_length);

            picture_parameter_set->parse(error);
            if (error)
                return;

            m_picture_parameter_sets.push_back(picture_parameter_set);
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    error box_error_code() const override
    {
        return error::invalid_avcc_box;
    }

    std::string type() const override
    {
        return TYPE;
    }

    std::string box_describe() const override
    {
        std::stringstream ss;
        ss << "  configuration_version: " << (uint32_t)m_configuration_version
           << std::endl;
        ss << "  avc_profile_indication: " << (uint32_t)m_avc_profile_indication
           << std::endl;
        ss << "  profile_compatibility: " << (uint32_t)m_profile_compatibility
           << std::endl;
        ss << "  avc_level_indication: " << (uint32_t)m_avc_level_indication
           << std::endl;
        ss << "  length_size_minus_one: " << (uint32_t)m_length_size_minus_one
           << std::endl;
        ss << "  num_of_sequence_parameter_sets: "
           << (uint32_t) m_num_of_sequence_parameter_sets << std::endl;

        ss << "  sequence_parameter_sets:" << std::endl;
        for (const auto& sps : m_sequence_parameter_sets)
        {
            ss << "    " << sps->describe();
        }

        ss << "  num_of_picture_parameter_sets: "
           << (uint32_t) m_num_of_picture_parameter_sets << std::endl;
        ss << "  picture_parameter_sets:" << std::endl;
        for (const auto& pps : m_picture_parameter_sets)
        {
            ss << "    " << pps->describe();
        }
        return ss.str();
    }

    std::shared_ptr<petro::sequence_parameter_set> sequence_parameter_set(
        uint32_t index) const
    {
        assert(index < m_sequence_parameter_sets.size());
        return m_sequence_parameter_sets[index];
    }

    std::shared_ptr<petro::picture_parameter_set> picture_parameter_set(
        uint32_t index) const
    {
        assert(index < m_picture_parameter_sets.size());
        return m_picture_parameter_sets[index];
    }

    uint8_t length_size() const
    {
        return m_length_size_minus_one + 1;
    }

private:

    /// always 1, if not, the decoder should not decode the stream.
    uint8_t m_configuration_version;

    /// profile code in ISO IEC 14496-10
    uint8_t m_avc_profile_indication;

    /// exactly the same byte that occurs between profile_IDC and level_IDC
    /// in the SPS
    uint8_t m_profile_compatibility;

    /// the level code
    uint8_t m_avc_level_indication;

    /// the length in bytes of the NALUnitLength field in an AVC video
    /// sample (subtracted by one).
    uint8_t m_length_size_minus_one;

    /// the number of SPSs that are used as the initial set of SPSs for
    /// decoding the AVC elementary stream
    uint8_t m_num_of_sequence_parameter_sets;

    /// the SPSs
    std::vector<std::shared_ptr<petro::sequence_parameter_set>>
    m_sequence_parameter_sets;

    /// the number of PPS that are used as the initial set of PPSs for
    /// decoding the AVC elementary stream
    uint8_t m_num_of_picture_parameter_sets;

    /// the SPSs
    std::vector<std::shared_ptr<petro::picture_parameter_set>>
    m_picture_parameter_sets;
};

}
}
