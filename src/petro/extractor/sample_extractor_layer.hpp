// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <string>

#include "../box/stco.hpp"
#include "../box/stsc.hpp"
#include "../box/stsz.hpp"
#include "../box/co64.hpp"

namespace petro
{
namespace extractor
{
    template<class Super>
    class sample_extractor_layer : public Super
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

            std::vector<uint64_t> chunk_offsets;

            auto stco = trak->template get_child<box::stco>();
            if (stco != nullptr)
            {
                chunk_offsets.resize(stco->entry_count());
                std::copy(stco->entries().begin(), stco->entries().end(),
                          chunk_offsets.begin());
            }
            else
            {
                auto co64 = trak->template get_child<box::co64>();
                if (co64 == nullptr)
                {
                    close();
                    return false;
                }

                chunk_offsets.resize(co64->entry_count());
                std::copy(co64->entries().begin(), co64->entries().end(),
                          chunk_offsets.begin());
            }

            auto stsc = trak->template get_child<box::stsc>();
            if (stsc == nullptr)
            {
                close();
                return false;
            }

            auto stsz = trak->template get_child<box::stsz>();
            if (stsz == nullptr)
            {
                close();
                return false;
            }

            reset();

            m_chunk_offsets = chunk_offsets;

            m_stsc = stsc;
            m_stsz = stsz;

            return true;
        }

        void close()
        {
            m_chunk_offsets.clear();
            m_stsc.reset();
            m_stsz.reset();

            Super::close();
        }

        void reset()
        {
            m_sample_index = 0;
            m_chunk_index = 0;
            m_chunk_sample = 0;
        }

        void advance()
        {
            m_sample_index += 1;
            m_chunk_sample += 1;
            if (m_chunk_sample >= m_stsc->samples_for_chunk(m_chunk_index))
            {
                m_chunk_index += 1;
                m_chunk_sample = 0;
            }
        }

        bool at_end() const
        {
            assert(m_stsz != nullptr);
            return (m_sample_index >= m_stsz->sample_count());
        }

        const uint8_t* sample_data() const
        {
            assert(m_chunk_offsets.size() != 0);
            return Super::data() +  m_chunk_offsets[m_chunk_index];
        }

        uint32_t sample_size() const
        {
            assert(m_stsz != nullptr);
            return m_stsz->sample_size(m_sample_index);
        }

        uint32_t sample_index() const
        {
            return m_sample_index;
        }

    private:

        uint32_t m_sample_index = 0;
        uint32_t m_chunk_index = 0;
        uint32_t m_chunk_sample = 0;

        std::vector<uint64_t> m_chunk_offsets;

        std::shared_ptr<const box::stsc> m_stsc;
        std::shared_ptr<const box::stsz> m_stsz;

    };
}
}
