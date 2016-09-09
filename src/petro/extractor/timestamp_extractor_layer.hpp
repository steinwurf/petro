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
    template<class Super>
    class timestamp_extractor_layer : public Super
    {
    public:

        bool open(const std::string& filename)
        {
            if (!Super::open(filename))
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

        void close()
        {
            m_timescale = 0;
            m_stts.reset();
            m_ctts.reset();

            Super::close();
        }

        uint64_t timestamp() const
        {
            return presentation_timestamp();
        }

        uint64_t decoding_timestamp() const
        {
            assert(m_stts != nullptr);
            return decoding_time(m_stts, m_timescale, Super::sample_index());
        }

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
