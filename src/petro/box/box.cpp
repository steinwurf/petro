// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "box.hpp"
#include "../helper.hpp"

namespace petro
{
namespace box
{
box::box(const std::string& type, std::weak_ptr<box> parent) :
    m_type(type),
    m_parent(parent),
    m_size(0),
    m_remaining_bytes(0),
    m_bs((uint8_t*)&m_size, 1)
{ }

box::box(const uint8_t* data, uint64_t size) :
    m_bs(data, size)
{ }

void box::read(uint32_t size, byte_stream& bs)
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

void box::parse(std::error_code& error)
{
    m_bs.seek(0, error);
    if (error)
        return;

    // size is an integer that specifies the number of bytes in this
    // box, including all its fields and contained boxes.
    uint32_t first_size;
    m_bs.read(first_size, error);
    if (error)
        return;

    // type identifies the box type; standard boxes use a compact type,
    // which is normally four printable characters,
    // to permit ease of identification,
    // and is shown so in the boxes below.
    // User extensions use an extended type; in this case,
    // the type field is set to "uuid".
    uint32_t type_value;
    m_bs.read(type_value, error);
    if (error)
        return;
    m_type = helper::type(type_value);

    uint64_t size = first_size;
    if (first_size == 1)
    {
        // if size is 1 then the actual size is in the field
        // largesize;
        m_bs.read<uint64_t>(size, error);
        if (error)
            return;
    }
    else if (first_size == 0)
    {
        // if size is 0, the box's content extends to the end of buffer.
        size = m_bs.size();
    }

    m_bs.resize(size);

    if (m_type == "uuid")
    {
        std::string extended_type = "";
        for (uint32_t i = 0; i < 16; ++i)
        {
            uint8_t c;
            m_bs.read(c, error);
            if (error)
                return;

            extended_type += c;
        }

        m_extended_type = extended_type;
    }

    parse_box_content(error);
    if (error)
        return;
}

void box::parse_box_content(std::error_code& error)
{
    assert(!error);
    m_bs.skip(m_bs.remaining_size(), error);
    if (error)
        return;
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
    if (m_bs.size() != 1)
    {
        return m_bs.size();
    }
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

void box::set_parent(std::weak_ptr<box> parent)
{
    m_parent = parent;
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
    auto child = shared_from_this();
    while (true)
    {
        for (auto c : child->m_children)
        {
            queue.push(c);
        }

        if (queue.empty())
            break;

        child = queue.front();
        queue.pop();

        if (child->m_type == type)
        {
            return child;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const box>> box::get_children(
    const std::string& type) const
{
    std::vector<std::shared_ptr<const box>> result;
    std::queue<std::shared_ptr<const box>> queue;
    auto child = shared_from_this();
    while (true)
    {
        for (auto c : child->m_children)
        {
            queue.push(c);
        }

        if (queue.empty())
            break;

        child = queue.front();
        queue.pop();

        if (child->m_type == type)
        {
            result.push_back(child);
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