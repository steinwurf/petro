// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "box.hpp"

namespace petro
{
    namespace box
    {
        box::box(const std::string& type, std::weak_ptr<box> parent) :
            m_type(type),
            m_parent(parent),
            m_size(0),
            m_remaining_bytes(0)
        { }

        void box::read(uint64_t size, byte_stream& bs)
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
                // (normally only used for a Media Data Box (mdat)).
                // This will not work for nested boxes (lucky mdat is never
                // nested)
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
                assert(m_remaining_bytes >= 16);
                for (uint32_t i = 0; i < 16; ++i)
                {
                    m_extended_type += bs.read_uint8_t();
                }
                m_remaining_bytes -= 16;
            }
        }

        std::string box::type() const
        {
            return m_type;
        }

        std::string box::extended_type() const
        {
            return m_extended_type;
        }

        uint64_t box::size() const
        {
            return m_size;
        }

        const std::vector<std::shared_ptr<box>> box::children() const
        {
            return m_children;
        }

        void box::add_child(std::shared_ptr<box> box)
        {
            m_children.push_back(box);
        }

        std::shared_ptr<box> box::parent() const
        {
            return m_parent.lock();
        }

        std::shared_ptr<box> box::get_parent(const std::string& type) const
        {
            auto parent = m_parent.lock();
            if (parent == nullptr || parent->type() == type)
            {
                return parent;
            }
            return parent->get_parent(type);
        }

        std::shared_ptr<const box> box::get_child(const std::string& type) const
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

        std::vector<std::shared_ptr<const box>> box::get_children(
            const std::string& type) const
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

        std::string box::describe() const
        {
            std::stringstream ss;
            ss << type() << std::endl;
            ss << "  size: " << size();
            return ss.str();
        }
    }
}