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

#include "base_box.hpp"

#include "../error.hpp"
#include "../stream_error_code_wrapper.hpp"

namespace petro
{
namespace box
{
class box : public base_box
{
public:

    box(const uint8_t* data, uint64_t size);

    void parse(std::error_code& error);

    virtual void parse_box_content(std::error_code& error);

    std::string type() const;

    std::string extended_type() const;

    uint64_t size() const;

    void set_parent(std::weak_ptr<base_box> parent);

    virtual std::string describe() const;

    virtual error box_error_code() const;

protected:

    std::string m_type;

    std::string m_extended_type;

    stream_error_code_wrapper m_bs;
};
}
}
