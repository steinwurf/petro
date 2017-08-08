// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "box.hpp"

namespace petro
{
namespace box
{
box::box(const uint8_t* data, uint64_t size) :
    m_bs(data, size)
{
    m_bs.set_error_code(petro::error::invalid_box);
}

void box::parse(std::error_code& error)
{
    m_bs.seek(0, error);
    if (error)
        return;

    // size is an integer that specifies the number of bytes in this
    // box, including all its fields and contained boxes.
    uint32_t first_size = 0;
    m_bs.read(first_size, error);
    if (error)
        return;

    // type identifies the box type; standard boxes use a compact type,
    // which is normally four printable characters,
    // to permit ease of identification,
    // and is shown so in the boxes below.
    // User extensions use an extended type; in this case,
    // the type field is set to "uuid".
    m_bs.read_type(m_type, error);
    if (error)
        return;

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

    if (size > m_bs.size())
    {
        error = std::make_error_code(std::errc::value_too_large);
        return;
    }
    // "Resize" stream_reader
    auto position = m_bs.position();
    m_bs = stream_error_code_wrapper(m_bs.data(), size);
    m_bs.seek(position, error);
    if (error)
        return;

    if (m_type == "uuid")
    {
        m_bs.read(m_extended_type, 16, error);
        if (error)
            return;
    }

    m_bs.set_error_code(box_error_code());
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

error box::box_error_code() const
{
    return error::invalid_box;
}
}
}