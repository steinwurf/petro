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

#include "box.hpp"

#include "../error.hpp"
#include "../stream.hpp"

namespace petro
{
namespace box
{
class data_box : public box
{
public:

    data_box(const uint8_t* data, uint64_t size);

    void parse(std::error_code& error);

    virtual void parse_box_content(std::error_code& error);

    std::string extended_type() const;

    uint64_t size() const;

    void set_parent(std::weak_ptr<box> parent);

    std::string describe() const override final;

protected:

    virtual std::string box_describe() const;

    virtual error box_error_code() const;

protected:

    std::string m_type;

    std::string m_extended_type;

    stream m_bs;
};
}
}
