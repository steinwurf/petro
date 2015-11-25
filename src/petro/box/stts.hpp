// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// (decoding) time-to-sample
    class stts : public full_box
    {

    public:

        struct entry_type
        {
            /// an integer that counts the number of consecutive samples that
            /// have the given duration.
            uint32_t sample_count;
            /// an integer that gives the delta of these samples in the
            /// time-scale of the media.
            uint32_t sample_delta;
        };

    public:

        static const std::string TYPE;

    public:
        stts(std::weak_ptr<box> parent):
            full_box(stts::TYPE, parent)
        { }

        void read(uint32_t size, byte_stream& bs)
        {
            full_box::read(size, bs);
            m_entry_count = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                m_entries.push_back(entry_type{
                    bs.read_uint32_t(),
                    bs.read_uint32_t()
                });
                m_remaining_bytes -= 8;
            }
            bs.skip(m_remaining_bytes);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (count, delta): ";
            auto seperator = "";
            for (const auto& entry : m_entries)
            {
                ss << seperator;
                ss << "(" << entry.sample_count << ", "
                   << entry.sample_delta << ")";
                seperator =  ", ";
            }
            ss << std::endl;

            return ss.str();
        }

        const std::vector<entry_type>& entries() const
        {
            return m_entries;
        }

        uint32_t sample_delta(uint32_t sample_index) const
        {
            auto total_sample_count = 0U;
            // initialize this value to the value of the first entry
            auto previous_delta = m_entries[0].sample_delta;
            // loop until we have incremented the total_sample_count to a number
            // which is higher than the sample_index.
            for (const auto& entry : m_entries)
            {
                total_sample_count += entry.sample_count;
                if (total_sample_count > sample_index)
                    return previous_delta;

                // keep updating the delta - we know that we've got the right
                // delta in the previous iteration if the total sample count is
                // larger than the given sample index.
                previous_delta = entry.sample_delta;
            }
            // a too high sample index was given.
            assert(0);
            return 0;
        }

    private:

        /// an integer that counts the actual entries
        uint32_t m_entry_count;

        /// a vector containing each entry's sample count and sample delta.
        /// The sample count is an integer that counts the number of consecutive
        /// samples that have the given duration.
        /// The sample delta is an integer that gives the delta of these samples
        /// in the time-scale of the media.
        std::vector<entry_type> m_entries;
    };

    const std::string stts::TYPE = "stts";
}
}
