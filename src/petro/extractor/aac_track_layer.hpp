// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include "../box/box.hpp"
#include "../box/esds.hpp"

namespace petro
{
namespace extractor
{
/// This layer exposes information about the aac track. This information is
/// used by the sample extractor layer to extract the aac samples.
template<class Super>
class aac_track_layer : public Super
{
public:

    /// open this and the underlying layer, returns false upon failure.
    void open(std::error_code& error)
    {
        assert(!error);
        Super::open(error);
        if (error)
        {
            Super::close();
            return;
        }

        auto root = Super::root();

        auto mp4a = root->get_child("mp4a");
        if (mp4a == nullptr)
        {
            error = petro::error::mp4a_box_missing;
            Super::close();
            return;
        }

        auto trak = mp4a->get_parent("trak");
        if (trak == nullptr)
        {
            error = petro::error::trak_box_missing;
            Super::close();
            return;
        }

        auto esds = mp4a->template get_child<box::esds>();
        if (esds == nullptr)
        {
            error = petro::error::esds_box_missing;
            Super::close();
            return;
        }

        m_trak = trak;

        auto descriptor =
            esds->descriptor()->
            decoder_config_descriptor()->
            decoder_specific_info_descriptor();

        if (descriptor == nullptr)
        {
            error = petro::error::descriptor_missing;
            Super::close();
            return;
        }

        m_mpeg_audio_object_type = descriptor->mpeg_audio_object_type();
        m_frequency_index = descriptor->frequency_index();
        m_channel_configuration = descriptor->channel_configuration();
    }

    /// close this and the underlying layer
    void close()
    {
        m_trak.reset();
        m_mpeg_audio_object_type = 0;
        m_frequency_index = 0;
        m_channel_configuration = 0;
        Super::close();
    }

    /// Returns a shared pointer to the AAC trak box.
    std::shared_ptr<const box::base_box> trak() const
    {
        assert(m_trak != nullptr);
        return m_trak;
    }

    /// Returns the MPEG audio object type
    uint8_t mpeg_audio_object_type() const
    {
        return m_mpeg_audio_object_type;
    }

    /// Returns the frequncy index
    uint32_t frequency_index() const
    {
        return m_frequency_index;
    }

    /// Returns the channel configuration
    uint8_t channel_configuration() const
    {
        return m_channel_configuration;
    }

private:

    std::shared_ptr<const box::base_box> m_trak;

    uint8_t m_mpeg_audio_object_type = 0;
    uint32_t m_frequency_index = 0;
    uint8_t m_channel_configuration = 0;
};
}
}
