// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "descriptor.hpp"
#include "decoder_specific_info_descriptor.hpp"

namespace petro
{
namespace descriptor
{
class decoder_config_descriptor : public descriptor
{

public:

    using decoder_specific_info_descriptor_type =
        std::shared_ptr<petro::descriptor::decoder_specific_info_descriptor>;

public:


    decoder_config_descriptor(const uint8_t* data, uint64_t size) :
        descriptor(data, size)
    { }

    void parse_descriptor_content(std::error_code& error) override
    {
        assert(!error);
        if (m_tag != tag::decoder_config)
        {
            error = std::make_error_code(std::errc::not_supported);
            return;
        }

        m_bs.read(m_object_type_id, error);
        if (error)
            return;

        /// This is unknown territory
        m_bs.skip(4, error);
        if (error)
            return;

        m_bs.read(m_max_bitrate, error);
        if (error)
            return;

        m_bs.read(m_average_bitrate, error);
        if (error)
            return;

        /// For some reason the decoder_specific_info_descriptor is optional
        if (m_bs.remaining_size() == 0)
            return;

        m_decoder_specific_info_descriptor =
            std::make_shared<decoder_specific_info_descriptor_type::element_type>(
                m_bs.remaining_data(), m_bs.remaining_size());
        m_decoder_specific_info_descriptor->parse(error);
        if (error)
            return;
        m_bs.skip(m_decoder_specific_info_descriptor->size(), error);
        if (error)
            return;
        // now 0-many profile level indication index descriptor
        while (m_bs.remaining_size() != 0)
        {
            auto profile_level_indication_index_descriptor =
                std::make_shared<descriptor>(
                    m_bs.remaining_data(), m_bs.remaining_size());
            m_decoder_specific_info_descriptor->parse(error);
            if (error)
                return;
            m_bs.skip(m_decoder_specific_info_descriptor->size(), error);
            if (error)
                return;

            m_profile_level_indication_index_descriptors.push_back(
                profile_level_indication_index_descriptor);
        }

        m_bs.skip(m_bs.remaining_size(), error);
        if (error)
            return;
    }

    uint8_t object_type_id() const
    {
        return m_object_type_id;
    }

    uint32_t max_bitrate() const
    {
        return m_max_bitrate;
    }

    uint32_t average_bitrate() const
    {
        return m_average_bitrate;
    }

    decoder_specific_info_descriptor_type
    decoder_specific_info_descriptor() const
    {
        return m_decoder_specific_info_descriptor;
    }

    const std::vector<std::shared_ptr<descriptor>>&
    profile_level_indication_index_descriptors() const
    {
        return m_profile_level_indication_index_descriptors;
    }

private:

    uint8_t m_object_type_id = 0;
    uint32_t m_max_bitrate = 0;
    uint32_t m_average_bitrate = 0;
    decoder_specific_info_descriptor_type m_decoder_specific_info_descriptor;
    std::vector<std::shared_ptr<descriptor>>
        m_profile_level_indication_index_descriptors;
};
}
}
