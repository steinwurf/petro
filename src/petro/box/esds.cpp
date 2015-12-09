// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include "esds.hpp"

#include "../descriptor/elemetary_stream_descriptor.hpp"

#include <string>
#include <memory>
#include <cstdint>

namespace petro
{
namespace box
{
    const std::string esds::TYPE = "esds";

    esds::esds(std::weak_ptr<box> parent):
        full_box(esds::TYPE, parent)
    { }

    void esds::read(uint64_t size, byte_stream& bs)
    {
        full_box::read(size, bs);

        auto elemetary_stream_descriptor_tag = bs.read_uint8_t();
        m_remaining_bytes -= 1;

        m_descriptor =
            std::make_shared<descriptor::elemetary_stream_descriptor>(
                bs, elemetary_stream_descriptor_tag);
        m_remaining_bytes -= m_descriptor->size();
    }

    std::string esds::describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        return ss.str();
    }

    esds::descriptor_type esds::descriptor() const
    {
        return m_descriptor;
    }
}
}
