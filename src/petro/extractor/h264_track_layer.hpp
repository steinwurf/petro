// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <memory>

#include "../box/box.hpp"
#include "../box/avcc.hpp"

namespace petro
{
namespace extractor
{
    template<class Super>
    class h264_track_layer : public Super
    {
    public:

        bool open(const std::string& filename)
        {
            if (!Super::open(filename))
            {
                Super::close();
                return false;
            }

            auto root = Super::root();

            auto avc1 = root->get_child("avc1");
            if (avc1 == nullptr)
            {
                Super::close();
                return false;
            }

            auto avcc = avc1->template get_child<box::avcc>();
            if (avcc == nullptr)
            {
                Super::close();
                return false;
            }

            auto trak = avc1->template get_parent("trak");
            if (trak == nullptr)
            {
                Super::close();
                return false;
            }
            m_avcc = avcc;
            m_trak = trak;
            return true;
        }

        std::shared_ptr<const box::box> trak() const
        {
            assert(m_trak != nullptr);
            return m_trak;
        }

        const uint8_t* pps_data(uint32_t index) const
        {
            assert(m_avcc != nullptr);
            return m_avcc->picture_parameter_set(index)->data();
        }

        uint32_t pps_size(uint32_t index) const
        {
            assert(m_avcc != nullptr);
            return m_avcc->picture_parameter_set(index)->size();
        }

        const uint8_t* sps_data(uint32_t index) const
        {
            assert(m_avcc != nullptr);
            return m_avcc->sequence_parameter_set(index)->data();
        }

        uint32_t sps_size(uint32_t index) const
        {
            assert(m_avcc != nullptr);
            return m_avcc->sequence_parameter_set(index)->size();
        }

        uint8_t nalu_length_size() const
        {
            assert(m_avcc != nullptr);
            return m_avcc->length_size();
        }

        void close()
        {
            m_trak.reset();
            m_avcc.reset();
        }

    private:

        std::shared_ptr<const box::box> m_trak;
        std::shared_ptr<const box::avcc> m_avcc;
    };
}
}
