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
box::box(const uint8_t* data, uint64_t size) :
    m_bs(data, size)
{ }

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
    assert(!m_type.empty() && "Run parse before getting type");
    return m_type;
}

std::string box::extended_type() const
{
    assert(!m_extended_type.empty() && "Run parse before getting extended_type");
    return m_extended_type;
}

uint64_t box::size() const
{
    return m_bs.size();
}

void box::set_parent(std::weak_ptr<base_box> parent)
{
    m_parent = parent;
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