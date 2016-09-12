// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

namespace petro
{
namespace extractor
{
    template<class Super>
    class h264_nalu_extractor_layer : public Super
    {
    public:

        bool open()
        {
            if (!Super::open())
            {
                Super::close();
                return false;
            }
            reset();
            return true;
        }

        void advance()
        {
            assert(!at_end());

            // increment the offset by the size of the previous nalu.
            m_sample_offset += m_nalu_size;

            // If we are at the end of this sample ...
            if (m_sample_offset == Super::sample_size())
            {
                // ... reset state
                m_sample_offset = 0;
                m_nalu_size = 0;

                // and get the next one
                Super::advance();

                // only read the nalu size of we are not at the end.
                if (at_end())
                    return;
            }
            read_nalu_size();
        }

        bool at_end() const
        {
            return Super::at_end() && m_sample_offset == 0;
        }

        void reset()
        {
            Super::reset();
            m_sample_offset = 0;

            // only read the nalu size if there's actually any samples to read.
            if (!Super::at_end())
                read_nalu_size();
        }

        const uint8_t* nalu_data() const
        {
            assert(!at_end());
            return m_sample_offset + Super::sample_data();
        }

        uint32_t nalu_size() const
        {
            assert(!at_end());
            return m_nalu_size;
        }

        bool is_new_sample() const
        {
            return m_sample_offset == Super::nalu_length_size();
        }

    private:

        void read_nalu_size()
        {
            assert(!at_end());
            auto length_size = Super::nalu_length_size();
            auto data = m_sample_offset + Super::sample_data();
            m_sample_offset += length_size;


            m_nalu_size = 0;
            for (uint8_t i = 0; i < length_size; ++i)
            {
                m_nalu_size |= ((uint32_t)data[i]) << ((length_size - 1) - i) * 8;
            }
        }

    private:

        uint32_t m_sample_offset = 0;
        uint32_t m_nalu_size = 0;
    };
}
}
