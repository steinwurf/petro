// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <queue>

#include <endian/big_endian.hpp>
#include <endian/stream_reader.hpp>

#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
class box : public std::enable_shared_from_this<box>
{
public:

    box(const uint8_t* data, uint64_t size);

    box(const std::string& type, std::weak_ptr<box> parent);

    virtual void read(uint32_t size, byte_stream& bs);

    void parse(std::error_code& error);

    virtual void parse_box_content(std::error_code& error);

    virtual std::string type() const;

    virtual std::string extended_type() const;

    uint64_t size() const;

    const std::vector<std::shared_ptr<box>> children() const;

    void add_child(std::shared_ptr<box> box);

    void set_parent(std::weak_ptr<box> parent);

    std::shared_ptr<box> parent() const;

    std::shared_ptr<box> get_parent(const std::string& type) const;

    std::shared_ptr<const box> get_child(const std::string& type) const;

    std::vector<std::shared_ptr<const box>> get_children(
        const std::string& type) const;

    virtual std::string describe() const;

public:

    template<class Child>
    std::shared_ptr<const Child> get_child() const
    {
        return std::dynamic_pointer_cast<const Child>(
            get_child(Child::TYPE));
    }

protected:

    std::string m_type;

    std::weak_ptr<box> m_parent;

    uint64_t m_size;

    uint64_t m_remaining_bytes;

    std::string m_extended_type;

    std::vector<std::shared_ptr<box>> m_children;

    endian::stream_reader<endian::big_endian> m_bs;
};
}
}