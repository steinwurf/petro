// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <queue>

#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    class box : public std::enable_shared_from_this<box>
    {
    public:

        box(const std::string& type, std::weak_ptr<box> parent):
            m_type(type),
            m_parent(parent),
            m_size(0),
            m_remaining_bytes(0)
        { }

        virtual void read(uint64_t size, byte_stream& bs)
        {
            m_size = size;

            if (m_size == 1)
            {
                // if size is 1 then the actual size is in the field
                // largesize;
                m_size = bs.read_uint64_t();
                m_remaining_bytes = m_size;
                m_remaining_bytes -= 8;
            }
            else if (m_size == 0)
            {
                // if size is 0, then this box is the last one in the file,
                // and its contents extend to the end of the file
                // (normally only used for a Media Data Box (mdat))
                m_size = bs.remaining_bytes();
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

        uint64_t size() const
        {
            return m_size;
        }

        const std::vector<std::shared_ptr<box>> children() const
        {
            return m_children;
        }

        void add_child(std::shared_ptr<box> box)
        {
            m_children.push_back(box);
        }

        std::shared_ptr<box> parent() const
        {
            return m_parent.lock();
        }

        std::shared_ptr<box> get_parent(const std::string& type) const
        {
            auto parent = m_parent.lock();
            if (parent == nullptr || parent->type() == type)
            {
                return parent;
            }
            return parent->get_parent(type);
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
        std::shared_ptr<const box> get_child(const std::string& type) const
        {
            std::queue<std::shared_ptr<const box>> queue;
            queue.push(shared_from_this());
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

        std::vector<std::shared_ptr<const box>> get_children(const std::string& type)
        {
            std::vector<std::shared_ptr<const box>> result;
            std::queue<std::shared_ptr<const box>> queue;
            queue.push(shared_from_this());
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

    protected:

        std::string m_type;

        std::weak_ptr<box> m_parent;

        uint64_t m_size;

        uint64_t m_remaining_bytes;

        std::string m_extended_type;

        std::vector<std::shared_ptr<box>> m_children;

    };
}
}