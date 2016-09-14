// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include <iostream>

namespace petro
{
namespace extractor
{
    /// This layer is used for extracting the nalu sample segments from the h264
    /// samples.
    /// h264 samples consists of one or more nalu samples, each preceded by a
    /// variable lengthed size:
    /// 1-4 bytes  <- size
    /// [size] bytes <- nalu data
    /// ... zero or more times ....
    /// Use the is_new_sample member function to determine which samples are
    /// related to which timestamps (timestamps are associated with
    /// h264 samples - not nalu samples).
    template<class Super>
    class h264_nalu_extractor_layer : public Super
    {
    public:

        /// Open this and the underlying layer, returns false upon failure.
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

        /// Advances the extractor to the next sample.
        void advance()
        {
            assert(!at_end());

            // increment the offset by the size of the previous nalu.
            std::cout << (uint64_t)nalu_data() << " ";
            std::cout << m_nalu_size << " ";
            m_sample_offset += m_nalu_size;
            std::cout << m_sample_offset << " " << Super::sample_size() << std::endl;
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

            std::cout << " nalu" << std::endl;
            read_nalu_size();
        }

        /// Returns true if no more samples are available.
        bool at_end() const
        {
            return Super::at_end() && m_sample_offset == 0;
        }

        /// Resets the extractor from the beginning.
        void reset()
        {
            Super::reset();
            m_sample_offset = 0;

            // only read the nalu size if there's actually any samples to read.
            if (!Super::at_end())
                read_nalu_size();
        }

        /// Returns a pointer to the nalu data.
        const uint8_t* nalu_data() const
        {
            assert(!at_end());
            return m_sample_offset + Super::sample_data();
        }

        /// Returns the size of the nalu data.
        uint32_t nalu_size() const
        {
            assert(!at_end());
            return m_nalu_size;
        }

        /// Returns true if this is a new h264 sample.
        bool is_new_sample() const
        {
            return m_sample_offset == Super::nalu_length_size();
        }

    private:

        /// Read the size of the nalu.
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
