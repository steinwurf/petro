// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <vector>

namespace petro
{
namespace box
{
    class box
    {
    public:

        box(const std::string& type, box* parent):
            m_type(type),
            m_parent(parent)
        { }

        const std::string& type() const
        {
            return m_type;
        }

        const std::vector<box*> children() const
        {
            return m_children;
        }

        const box* parent() const
        {
            return m_parent;
        }

        virtual std::string describe() const
        {
            return type();
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

        box* m_parent;

        std::vector<box*> m_children;

    };
}
}