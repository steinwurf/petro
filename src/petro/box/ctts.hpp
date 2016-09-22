// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
    namespace box
    {
        /// (composition) time to sample
        class ctts : public full_box
        {
        public:

            struct entry_type
            {
                /// The sample count is an integer that counts the number of
                /// consecutive samples that have the given offset.
                uint32_t sample_count;

                /// The sample offset is a non-negative integer that gives the
                /// offset between composition time (CT) and decoding time (DT),
                /// such that CT(n) = DT(n) + CTTS(n).
                uint32_t sample_offset;
            };

        public:

            static const std::string TYPE;

        public:
            ctts(std::weak_ptr<box> parent) :
                full_box(ctts::TYPE, parent)
            { }

            void read(uint64_t size, byte_stream& bs)
            {
                full_box::read(size, bs);
                m_entry_count = bs.read_uint32_t();
                m_remaining_bytes -= 4;

                for (uint32_t i = 0; i < m_entry_count; ++i)
                {
                    uint32_t sample_count = bs.read_uint32_t();
                    uint32_t sample_offset = bs.read_uint32_t();
                    m_entries.push_back(entry_type
                    {
                        sample_count,
                        sample_offset
                    });
                    m_remaining_bytes -= 8;

                    for (uint32_t i = 0; i < sample_count; ++i)
                    {
                        m_composition_times.push_back(sample_offset);
                    }
                }

                assert(m_remaining_bytes == 0);
                bs.skip(m_remaining_bytes);
            }

            virtual std::string describe() const
            {
                std::stringstream ss;
                ss << full_box::describe() << std::endl;
                ss << "  entry_count: " << m_entry_count << std::endl;
                ss << "  entries (count, offset): ";
                auto seperator = "";
                uint32_t max_print = 5;
                for (uint32_t i = 0;
                     i < std::min((uint32_t)m_entries.size(), max_print); ++i)
                {
                    auto entry = m_entries[i];
                    ss << seperator;
                    ss << "(" << entry.sample_count << ", ";
                    ss << entry.sample_offset << ")";
                    seperator =  ", ";
                }
                if (m_entries.size() > max_print)
                    ss << "...";
                ss << std::endl;

                return ss.str();
            }

            uint32_t samples() const
            {
                return m_composition_times.size();
            }

            uint32_t composition_time(uint32_t sample_index) const
            {
                assert(sample_index < m_composition_times.size());
                return m_composition_times[sample_index];
            }

        private:
            /// an integer that counts the actual entries
            uint32_t m_entry_count;

            /// a vector containing each entry's sample count and sample offset.
            std::vector<entry_type> m_entries;

            /// the composition time for each sample.
            /// this field is not part of the standard, and is simply a result of
            /// the extraction of the data in the entries.
            std::vector<uint32_t> m_composition_times;
        };
    }
}
