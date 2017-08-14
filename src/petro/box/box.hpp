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

namespace petro
{
namespace box
{
class box : public std::enable_shared_from_this<box>
{
public:

    virtual std::string type() const = 0;

    const std::vector<std::shared_ptr<box>> children() const;

    void add_child(std::shared_ptr<box> box);

    std::shared_ptr<box> parent() const;

    std::shared_ptr<box> get_parent(const std::string& type) const;

    std::shared_ptr<const box> get_child(const std::string& type) const;

    std::vector<std::shared_ptr<const box>> get_children(
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

    std::weak_ptr<box> m_parent;
    std::vector<std::shared_ptr<box>> m_children;
};
}
}
