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

        /// Returns the samples timestamp, offset based on the looping
        uint64_t timestamp() const
        {
            return m_loop_offset + Super::timestamp();
        }

        /// Advances to the next sample
        void advance()
        {
            Super::advance();
            if (m_loop && Super::at_end())
            {
                m_loop_offset = timestamp();
                m_loops++;
                Super::reset();
            }
        }

        /// Returns the number of times the extractor has looped
        uint32_t loops() const
        {
            return m_loops;
        }

        /// Resets the extractor.
        void reset()
        {
            Super::reset();
            m_loop_offset = 0;
            m_loops = 0;
        }

    private:

        bool m_loop = true;
        uint64_t m_loop_offset = 0;
        uint32_t m_loops = 0;

    };
}
}