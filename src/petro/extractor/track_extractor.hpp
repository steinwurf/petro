// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <memory>

#include "../box/hdlr.hpp"
#include "../box/mdia.hpp"
#include "../box/minf.hpp"
#include "../box/moov.hpp"
#include "../box/root.hpp"
#include "../box/stbl.hpp"
#include "../box/stsd.hpp"
#include "../box/tkhd.hpp"
#include "../box/trak.hpp"
#include "../parser.hpp"

#include "track_type.hpp"

namespace petro
{
namespace extractor
{
/// Class for extracting tracks from a file.
class track_extractor
{
public:
    /// A parser for the different tracks
    using track_parser = parser<box::moov<parser<box::trak<parser<
        box::tkhd,
        box::mdia<parser<box::hdlr, // needed for esds extraction
                         box::minf<parser<box::stbl<parser<box::stsd>>>>>>>>>>>;

    /// Struct containing the track information
    struct track
    {
        /// ID of the track
        uint32_t id;
        /// The track type
        petro::extractor::track_type type;
    };

public:
    /// parses data, extracts the tracks and gathers them in the m_tracks vector
    void open(const uint8_t* data, uint64_t size, std::error_code& error)
    {
        assert(!error);
        track_parser parser;
        auto root = std::make_shared<petro::box::root>();
        parser.parse(data, size, root, error);
        if (error)
            return;

        m_tracks.clear();
        for (auto tkhd : root->template get_children<box::tkhd>())
        {
            auto trak = tkhd->parent();
            auto stsd = trak->template get_child<box::stsd>();
            auto booxy = stsd->children().at(0);
            auto type_str = booxy->type();

            auto type = petro::extractor::track_type::unknown;
            if (type_str == "mp4a")
            {
                type = petro::extractor::track_type::unknown_audio;
                auto esds = booxy->template get_child<box::esds>();
                if (esds != nullptr)
                {
                    auto config_descriptor =
                        esds->descriptor()->decoder_config_descriptor();

                    if (config_descriptor->object_type_id() == 0x40)
                    {
                        type = track_type::aac;
                    }
                }
            }
            else if (type_str == "avc1")
            {
                type = track_type::avc1;
            }
            else if (type_str == "hvc1")
            {
                type = track_type::hvc1;
            }
            else if (type_str == "text")
            {
                type = track_type::text;
            }
            m_tracks.push_back({tkhd->track_id(), type});
        }
    }

    /// Clears the tracks
    void close()
    {
        m_tracks.clear();
    }

    /// @return A vector containing the tracks extracted
    std::vector<track> tracks() const
    {
        return m_tracks;
    }

protected:
    /// Vector to contain the tracks extracted
    std::vector<track> m_tracks;
};
}
}
