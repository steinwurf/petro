// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace petro
{
namespace extractor
{
/// Helper layer allowing extractors to be looping.
/// Note this only works if the extraction only uses the timestamp function
/// for timestamps, or not at all, i.e., the looping will fail if the
/// extraction utilizes the decoding_timestamp or presentatino_timestamp.
template<class Super>
class looper_layer : public Super
{
public:

    /// Enables looping
    void enable_looping()
    {
        m_loop = true;
    }

    /// Disables looping
    void disable_looping()
    {
        m_loop = false;
    }

    /// Returns the number of times the extractor has looped
    uint32_t loops() const
    {
        return m_loops;
    }

    /// Returns the sample's decoding timestamp with the loop offset
    uint64_t decoding_timestamp() const
    {
        return m_loop_offset + Super::decoding_timestamp();
    }

    /// Returns the sample's presentation timestamp with the loop offset
    uint64_t presentation_timestamp() const
    {
        return m_loop_offset + Super::presentation_timestamp();
    }

    /// Advances to the next sample
    void advance()
    {
        Super::advance();
        if (m_loop && Super::at_end())
        {
            // Use the total media duration as a loop offset to keep audio and
            // video in sync. The media duration is the maximum of the lengths
            // of the video and audio tracks, so the offset will be the same
            // for video and audio extractors.
            m_loop_offset += Super::media_duration();
            m_loops++;
            Super::reset();
        }
    }

    /// Resets the extractor.
    void reset()
    {
        Super::reset();
        m_loop_offset = 0;
        m_loops = 0;
    }

private:

    bool m_loop = false;
    uint64_t m_loop_offset = 0;
    uint32_t m_loops = 0;
};
}
}
