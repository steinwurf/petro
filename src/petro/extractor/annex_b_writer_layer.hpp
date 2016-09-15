// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <vector>
#include <memory>

#include "read_bytes.hpp"

namespace petro
{
namespace extractor
{
    template<class Super>
    class annex_b_writer_layer : public Super
    {

    private:

        class avc_sample
        {

        public:

            avc_sample(uint32_t size_length, const uint8_t* data, uint32_t size)
            {
                while(size != 0)
                {
                    auto nalu_size = detail::read_bytes(size_length, data);
                    data += size_length;
                    size -= size_length;
                    m_nalus.push_back(data);
                    data += nalu_size;
                    size -= nalu_size;
                    m_nalu_sizes.push_back(nalu_size);
                }
            }

            uint32_t nalu_count() const
            {
                return m_nalus.size();
            }

            const uint8_t* nalu_at(uint32_t index) const
            {
                assert(index < m_nalus.size());
                return m_nalus[index];
            }

            uint32_t nalu_size_at(uint32_t index) const
            {
                assert(index < m_nalu_sizes.size());
                return m_nalu_sizes[index];
            }

        private:

            std::vector<const uint8_t*> m_nalus;
            std::vector<uint32_t> m_nalu_sizes;
        };

    public:

        bool open()
        {
            if (!Super::open())
            {
                Super::close();
                return false;
            }
            update_state();
            return true;
        }

        void reset()
        {
            Super::reset();
            update_state();
        }

        void advance()
        {
            Super::advance();
            update_state();
        }

        void write_annex_b(uint8_t* buffer) const
        {
            for (uint32_t i = 0; i < m_avc_sample->nalu_count(); ++i)
            {
                Super::write_nalu_header(buffer);
                buffer += Super::nalu_header_size();

                auto nalu = m_avc_sample->nalu_at(i);
                auto nalu_size = m_avc_sample->nalu_size_at(i);
                std::copy(nalu, nalu + nalu_size, buffer);
                buffer += nalu_size;
            }
        }

        uint32_t annex_b_size() const
        {
            uint32_t size = 0;
            for (uint32_t i = 0; i < m_avc_sample->nalu_count(); ++i)
            {
                size += Super::nalu_header_size();
                size += m_avc_sample->nalu_size_at(i);
            }
            return size;
        }

        void update_state()
        {
            if (Super::at_end())
            {
                m_avc_sample.reset();
                return;
            }
            m_avc_sample = std::unique_ptr<avc_sample>(new avc_sample(
                Super::nalu_length_size(),
                Super::sample_data(),
                Super::sample_size()));
        }

    private:

        std::unique_ptr<avc_sample> m_avc_sample;
    };
}
}
