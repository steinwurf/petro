// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#include "esds.hpp"

#include <string>

namespace petro
{
namespace box
{
    const std::string esds::TYPE = "esds";


    esds::esds(std::weak_ptr<box> parent):
        box(esds::TYPE, parent)
    { }


    uint8_t esds::read_section_length(byte_stream& bs)
    {
        // The next byte is either the section length or the first of
        // a three bytes optional extended descriptor type tag string.
        // The tag types are 0x80,0x81,0xFE.
        auto section_length = bs.read_uint8_t();
        m_remaining_bytes -= 1;

        if (section_length == 0x80 ||
            section_length == 0x81 ||
            section_length == 0xFE)
        {
            // appearently we are reading the optional extended descriptor type
            // tag string - let's throw it away...
            bs.skip(2);
            m_remaining_bytes -= 2;
            // ... and read the section length.
            section_length = bs.read_uint8_t();
            m_remaining_bytes -= 1;
        }
        return section_length;
    }

    void esds::read(uint64_t size, byte_stream& bs)
    {
        box::read(size, bs);
        m_version = bs.read_uint32_t();
        m_remaining_bytes -= 4;
        auto section = bs.read_uint8_t();
        m_remaining_bytes -= 1;

        // we currently only handle section 3(?)
        if (section == 3)
        {
            auto section_length = read_section_length(bs);
            std::cout << (uint32_t)section_length << std::endl;
            std::cout << m_remaining_bytes << std::endl;
            m_es_id = bs.read_uint16_t();
            m_remaining_bytes -= 2;
            m_byte_stream_priority = bs.read_uint8_t();
            m_remaining_bytes -= 1;
            assert(m_byte_stream_priority < 31);

            section = bs.read_uint8_t();
            m_remaining_bytes -= 1;
        }

        if (section == 4)
        {
            auto section_length = read_section_length(bs);
            std::cout << (uint32_t)section_length << std::endl;
            std::cout << m_remaining_bytes << std::endl;
            auto byte_object_type_ID = bs.read_uint8_t();
            m_remaining_bytes -= 1;
            auto d = bs.read_uint8_t();
            m_remaining_bytes -= 1;
            auto stream_type = d & 0b00111111;
            auto upstream = d & 0b01000000;
            auto reserved = d & 0b10000000;
            assert(reserved == 0);

            uint32_t buffer_size =
                (uint32_t) bs.read_uint8_t() << 16 |
                (uint32_t) bs.read_uint8_t() << 8 |
                (uint32_t) bs.read_uint8_t();
            m_remaining_bytes -= 3;

            auto maximum_bit_rate = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            auto average_bit_rate = bs.read_uint32_t();
            m_remaining_bytes -= 4;
            section = bs.read_uint8_t();
            m_remaining_bytes -= 1;
        }

        std::cout << (uint32_t)section << std::endl;

        if (section == 5)
        {
            auto section_length = read_section_length(bs);
            std::cout << (uint32_t)section_length << std::endl;
            std::cout << m_remaining_bytes << std::endl;

            auto audio_profile_id = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            auto no_of_channels = bs.read_uint8_t();
            m_remaining_bytes -= 1;

            section = bs.read_uint8_t();
            m_remaining_bytes -= 1;
        }

        if (section == 6)
        {
            auto section_length = read_section_length(bs);
            auto sl_value = bs.read_uint8_t();
            m_remaining_bytes -= 1;
        }


        bs.skip(m_remaining_bytes);
    }

    std::string esds::describe() const
    {
        std::stringstream ss;
        ss << box::describe() << std::endl;
        ss << "  version: " << m_version << std::endl;
        ss << "  es_id: " << (uint32_t)m_es_id << std::endl;
        ss << "  byte_stream_priority: " << (uint32_t)m_byte_stream_priority << std::endl;
        ss << "  data: " << m_data << std::endl;
        return ss.str();
    }
}
}
