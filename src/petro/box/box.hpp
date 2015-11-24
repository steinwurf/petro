// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <queue>

namespace petro
{
namespace box
{
    class box
    {
    public:

        box(const std::string& type):
            m_type(type),
            m_size(0),
            m_remaining_bytes(0),
            m_parent(nullptr)
        { }

        virtual void read(uint32_t size, byte_stream& bs, box* parent)
        {
            m_size = size;
            m_parent = parent;

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

            if (m_type == "uuid")
            {
                for (int i = 0; i < 16; ++i)
                {
                    m_extended_type += bs.read_uint8_t();
                    m_remaining_bytes -= 1;
                }
            }
        }

        virtual std::string type() const
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

        void add_child(box* box)
        {
            m_children.push_back(box);
        }

        const box* parent() const
        {
            return m_parent;
        }

        const box* get_parent(const std::string& type) const
        {
            if (m_parent == nullptr || m_parent->type() == type)
            {
                return m_parent;
            }
            return m_parent->get_parent(type);
        }

        /// Find the first child matching the type.
        /// The child is found in a breadth-first search manner:
        /// (the numbers represent the order each node is visited.)
        /// .-----------------------------------------------------------------.
        /// |this:                                   .--------*--------.      |
        /// |                                       /         |         \     |
        /// |children:                           .-1-.        2       .-3-.   |
        /// |                                   /  |  \       |       |   |   |
        /// |children of children:             4   5 .-6-.  .-7-.     8  .9-. |
        /// |                                  |     |   |  |   |     |  |  | |
        /// |children of children's children: 10    11  12 13  14    15 16 17 |
        /// |(and so on...)                                                   |
        /// '-----------------------------------------------------------------'
        const box* get_child(const std::string& type) const
        {
            std::queue<const box*> queue;
            queue.push(this);
            while (!queue.empty())
            {
                auto child = queue.front();
                queue.pop();

                if (child->m_type == type)
                {
                    return child;
                }

                for (auto c : child->m_children)
                {
                    queue.push(c);
                }
            }
            return nullptr;
        }

        const std::vector<const box*> get_children(const std::string& type)
        {
            std::vector<const box*> result;
            std::queue<const box*> queue;
            queue.push(this);
            while (!queue.empty())
            {
                auto child = queue.front();
                queue.pop();

                if (child->m_type == type)
                {
                    result.push_back(child);
                }

                for (auto c : child->m_children)
                {
                    queue.push(c);
                }
            }
            return result;
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