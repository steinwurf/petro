// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "full_box.hpp"
#include "../byte_stream.hpp"
#include "../descriptor/elemetary_stream_descriptor.hpp"
namespace petro
{
namespace box
{
/// MPEG-4 Elementary Stream Descriptor
class esds : public full_box
{
public:

    static const std::string TYPE;

public:

    using descriptor_type =
        std::shared_ptr<descriptor::elemetary_stream_descriptor>;

public:
    esds(std::weak_ptr<box> parent);
    void read(uint32_t size, byte_stream& bs);
    std::string describe() const;

    descriptor_type descriptor() const;

private:

    descriptor_type m_descriptor;
};
}
}
