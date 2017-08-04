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

#include "base_box.hpp"

#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
class box : public base_box
{
public:

    box(const uint8_t* data, uint64_t size);

    virtual void read(uint32_t size, byte_stream& bs);

    void parse(std::error_code& error);

    virtual void parse_box_content(std::error_code& error);

    std::string type() const;

    std::string extended_type() const;

    uint64_t size() const;

    void set_parent(std::weak_ptr<base_box> parent);

    virtual std::string describe() const;

protected:

    std::string m_type;

    uint64_t m_size;

    uint64_t m_remaining_bytes;

    std::string m_extended_type;

    endian::stream_reader<endian::big_endian> m_bs;
};
}
}