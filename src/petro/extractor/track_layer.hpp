// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <system_error>
#include <boost/optional.hpp>

#include "../box/tkhd.hpp"

namespace petro
{
namespace extractor
{
/// Provides the stack with track information
template<class Super>
class track_layer : public Super
{
public:

    /// Opens the extractor. Remember to set the data buffer before calling
    /// this.
    void open(std::error_code& error)
    {
        assert(!error);
        Super::open(error);
        if (error)
        {
            Super::close();
            return;
        }

        assert(m_track_id);

        for (auto tkhd : Super::root()->template get_children<box::tkhd>())
        {
            if (tkhd->track_id() == *m_track_id)
                m_trak = tkhd->parent();
        }
        if (m_trak == nullptr)
            error = petro::error::trak_box_missing;
    }

    void close()
    {
        Super::close();
        m_track_id = boost::none;
        m_trak = nullptr;
    }

    void set_track_id(uint32_t track_id)
    {
        assert(m_trak == nullptr);
        m_track_id = track_id;
    }

    /// Returns a shared pointer to the AAC trak box.
    std::shared_ptr<const box::box> trak() const
    {
        assert(m_trak != nullptr);
        return m_trak;
    }

    uint32_t track_id() const
    {
        assert(m_track_id);
        return *m_track_id;
    }

private:

    boost::optional<uint32_t> m_track_id = boost::none;
    std::shared_ptr<const box::box> m_trak = nullptr;
};
}
}
