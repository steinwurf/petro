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
class base_box : public std::enable_shared_from_this<base_box>
{
public:

    virtual std::string type() const = 0;

    const std::vector<std::shared_ptr<base_box>> children() const;

    void add_child(std::shared_ptr<base_box> box);

    std::shared_ptr<base_box> parent() const;

    std::shared_ptr<base_box> get_parent(const std::string& type) const;

    std::shared_ptr<const base_box> get_child(const std::string& type) const;

    std::vector<std::shared_ptr<const base_box>> get_children(
        const std::string& type) const;

    virtual std::string describe() const = 0;

public:

    template<class Child>
    std::shared_ptr<const Child> get_child() const
    {
        return std::dynamic_pointer_cast<const Child>(
            get_child(Child::TYPE));
    }

protected:

    std::weak_ptr<base_box> m_parent;
    std::vector<std::shared_ptr<base_box>> m_children;
};
}
}