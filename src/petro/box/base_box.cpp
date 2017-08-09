// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "base_box.hpp"

namespace petro
{
namespace box
{

const std::vector<std::shared_ptr<base_box>> base_box::children() const
{
    return m_children;
}

void base_box::add_child(std::shared_ptr<base_box> box)
{
    m_children.push_back(box);
}

std::shared_ptr<base_box> base_box::parent() const
{
    return m_parent.lock();
}

std::shared_ptr<base_box> base_box::get_parent(const std::string& type) const
{
    auto parent = m_parent.lock();
    if (parent == nullptr || parent->type() == type)
    {
        return parent;
    }

    return parent->get_parent(type);
}

std::shared_ptr<const base_box> base_box::get_child(const std::string& type)
const
{
    std::queue<std::shared_ptr<const base_box>> queue;
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

        if (child->type() == type)
        {
            return child;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const base_box>> base_box::get_children(
    const std::string& type) const
{
    std::vector<std::shared_ptr<const base_box>> result;
    std::queue<std::shared_ptr<const base_box>> queue;
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

        if (child->type() == type)
        {
            result.push_back(child);
        }
    }
    return result;
}
}
}
