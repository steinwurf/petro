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
    template<class Super>
    class looper : public Super
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
            return m_loop_offset + Super::timestamp();
        }

        void advance()
        {
            Super::advance();
            if (m_loop && Super::at_end())
            {
                m_loop_offset = timestamp();
                Super::reset();
            }
        }

        void reset()
        {
            Super::reset();
            m_loop_offset = 0;
        }

    private:

        bool m_loop = true;
        uint64_t m_loop_offset = 0;

    };
}
}