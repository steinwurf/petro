// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace petro
{
namespace box
{
    class box
    {
    public:

        box(const std::string& type, uint32_t size, byte_stream& bs, box* parent):
            m_type(type),
            m_size(size),
            m_parent(parent)
        {
            if (m_size == 1)
            {
                // if size is 1 then the actual size is in the field
                // largesize;
                m_size = bs.read_uint64_t();
                m_remaining_bytes = m_size - 8;
            }
            else if (m_size == 0)
            {
                // if size is 0, then this box is the last one in the file,
                // and its contents extend to the end of the file
                // (normally only used for a Media Data Box (mdat))
                m_size = bs.size();
                m_remaining_bytes = m_size;
            }
            else
            {
                m_remaining_bytes = m_size;
            }

            // the size is included in size, so we need to accommodate
            // for this.
            m_remaining_bytes -= 4;

            // the type is included in size, so we need to accommodate for
            // this.
            m_remaining_bytes -= 4;

            if (type == "uuid")
            {
                for (int i = 0; i < 16; ++i)
                {
                    m_extended_type += bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }
            }
        }

        const std::string& type() const
        {
            return m_type;
        }

        uint32_t size() const
        {
            return m_size;
        }

        const std::vector<box*> children() const
        {
            return m_children;
        }

        const box* parent() const
        {
            return m_parent;
        }

        virtual std::string describe() const
        {
            std::stringstream ss;
            ss << type() << std::endl;
            ss << "  size: " << size();
            return ss.str();
        }

        virtual ~box()
        {
            for (auto box : m_children)
            {
                delete box;
            }
        }

    protected:

        std::string m_type;

        uint32_t m_size;

        uint32_t m_remaining_bytes;

        box* m_parent;

        std::string m_extended_type;

        std::vector<box*> m_children;

    };
}
}