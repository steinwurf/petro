// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "full_box.hpp"
#include "hdlr.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{


    const box* get_handler(const box* b)
    {
        const box* parent = b->parent();
        if (parent != nullptr)
        {
            std::cout << "looking for handler in " << parent->type() << std::endl;
            for (auto child : parent->children())
            {
                if (child->type() == "hdlr")
                    return child;
            }
            if (parent->parent() != nullptr)
            {
                return get_handler(parent);
            }
        }
        return nullptr;
    }

    /// sample descriptions (codec types, initialization etc.)
    class stsd : public full_box
    {

    public:

        static const std::string TYPE;

    public:
        stsd(byte_stream& bs, uint32_t size):
            full_box(stsd::TYPE, bs, size)
        {
            m_entry_count = bs.read_uint32_t();
            size -= 4;
            const box* handler = get_handler(this);
            for (uint32_t i = 0; i < m_entry_count; ++i)
            {
                // reserved
                bs.skip(6);
                size -= 6;

                m_entries.push_back(bs.read_uint16_t());
                size -= 2;
                if (handler != nullptr)
                {
                    std::string handler_type = ((hdlr*)handler)->handler_type();
                }
                else
                {
                    std::cout << "unable to find handler" << std::endl;
                }
            }
            bs.skip(size);
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << full_box::describe() << std::endl;
            ss << "  entry_count: " << m_entry_count << std::endl;
            ss << "  entries (data_reference_index): ";
            auto seperator = "";
            for (const auto& entry : m_entries)
            {
                ss << seperator;
                ss << "(" << entry << ")";
                seperator =  ", ";
            }
            ss << std::endl;
            return ss.str();
        }

    private:

        /// an integer that gives the number of entries
        uint32_t m_entry_count;
        std::vector<uint16_t> m_entries;
    };

    const std::string stsd::TYPE = "stsd";
}
}
