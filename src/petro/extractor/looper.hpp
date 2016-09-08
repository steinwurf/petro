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
    template<class Extractor>
    class looper : public Extractor
    {
    public:

        void enable_looping()
        {
            m_loop = true;
        }

        void disable_looping()
        {
            m_loop = false;
        }

        uint64_t timestamp() const
        {
            return m_loop_offset + Extractor::timestamp();
        }

        void advance()
        {
            Extractor::advance();
            if (m_loop && Extractor::at_end())
            {
                m_loop_offset = timestamp();
                Extractor::reset();
            }
        }

        void reset()
        {
            Extractor::reset();
            m_loop_offset = 0;
        }

    private:

        bool m_loop = true;
        uint64_t m_loop_offset = 0;

    };
}
}