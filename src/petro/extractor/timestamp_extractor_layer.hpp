// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <string>
#include "../decoding_time.hpp"
#include "../presentation_time.hpp"
#include "../box/mdhd.hpp"
#include "../box/stts.hpp"
#include "../box/ctts.hpp"

namespace petro
{
    namespace extractor
    {
        /// Uses the aac and avc track layers to extract sample timestamps.
        template<class Super>
        class timestamp_extractor_layer : public Super
        {
        public:

            /// Open this an the underlying layers
            bool open()
            {
                if (!Super::open())
                {
                    Super::close();
                    return false;
                }
                auto trak = Super::trak();

                auto mdhd = trak->template get_child<box::mdhd>();
                if (mdhd == nullptr)
                {
                    close();
                    return false;
                }
                m_timescale = mdhd->timescale();

                auto stts = trak->template get_child<box::stts>();
                if (stts == nullptr)
                {
                    close();
                    return false;
                }

                m_stts = stts;
                m_ctts = trak->template get_child<box::ctts>();

                return true;
            }

            /// Close this an the underlying layers
            void close()
            {
                m_timescale = 0;
                m_stts.reset();
                m_ctts.reset();

                Super::close();
            }

            /// Return the timestamp related to the current sample
            uint64_t timestamp() const
            {
                return presentation_timestamp();
            }

            /// Return the decoding timestamp related to the current sample
            uint64_t decoding_timestamp() const
            {
                assert(m_stts != nullptr);
                return decoding_time(m_stts, m_timescale, Super::sample_index());
            }

            /// Return the presentation timestamp related to the current sample
            uint64_t presentation_timestamp() const
            {
                assert(m_stts != nullptr);
                return presentation_time(
                           m_stts, m_ctts, m_timescale, Super::sample_index());
            }

        private:

            uint32_t m_timescale = 0;

            std::shared_ptr<const box::stts> m_stts;
            std::shared_ptr<const box::ctts> m_ctts;

        };
    }
}
