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
        full_box(esds::TYPE, parent)
    { }

    void esds::read(uint64_t size, byte_stream& bs)
    {
        full_box::read(size, bs);

        uint8_t tag = bs.read_uint8_t();
        m_remaining_bytes -= 1;

        m_descriptor = std::make_shared<elemetary_stream_descriptor>(bs, tag);
        std::cout << m_descriptor->remaining_bytes() << std::endl;

        bs.skip(m_descriptor->remaining_bytes());
        m_remaining_bytes -= m_descriptor->length();

        // // Read mp4 elementary stream descriptor
        // {
        //     auto tag = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        //     assert(tag == 3);

        //     auto descriptor_length = read_descriptor_length(bs);
        //     m_es_id = bs.read_uint16_t();
        //     m_remaining_bytes -= 2;

        //     auto d = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;

        //     auto stream_dependence_flag = (d & 0b00000001) == 1;
        //     auto url_flag = ((d & 0b00000010) >> 1) == 1;
        //     auto ocr_stream_flag = ((d & 0b00000100) >> 2) == 1;

        //     m_byte_stream_priority = d & 0b11111000 >> 3);

        //     if (stream_dependence_flag)
        //     {
        //         m_depends_on_esid = bs.read_uint16_t();
        //         m_remaining_bytes -= 2;
        //     }

        //     if (url_flag)
        //     {
        //         auto url_length = bs.read_uint8_t();
        //         m_remaining_bytes -= 1;

        //         for (uint8_t i = 0; i < url_length; ++i)
        //         {
        //             m_url += bs.read_uint8_t();
        //             m_remaining_bytes -= 1;
        //         }
        //     }

        //     if (ocr_stream_flag)
        //     {
        //         m_ocr_es_id = bs.read_uint16_t();
        //         m_remaining_bytes -= 2;
        //     }
        // }

        // // Read decoder configuration descriptor (this is stored inside the
        // // above mp4 ES)
        // {
        //     auto tag = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        //     assert(tag == 4);

        //     auto descriptor_length = read_descriptor_length(bs);

        //     auto byte_object_type_ID = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        //     auto d = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        //     auto stream_type = d & 0b00111111;
        //     auto upstream = d & 0b01000000;
        //     auto reserved = d & 0b10000000;
        //     assert(reserved == 0);

        //     uint32_t buffer_size =
        //         (uint32_t) bs.read_uint8_t() << 16 |
        //         (uint32_t) bs.read_uint8_t() << 8 |
        //         (uint32_t) bs.read_uint8_t();
        //     m_remaining_bytes -= 3;

        //     auto maximum_bit_rate = bs.read_uint32_t();
        //     m_remaining_bytes -= 4;
        //     auto average_bit_rate = bs.read_uint32_t();
        //     m_remaining_bytes -= 4;
        //     tag = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        // }

        // // std::cout << (uint32_t)tag << std::endl;

        // if (tag == 5)
        // {
        //     auto descriptor_length = read_descriptor_length(bs);
        //     // std::cout << (uint32_t)descriptor_length << std::endl;
        //     // std::cout << m_remaining_bytes << std::endl;

        //     auto audio_profile_id = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;

        //     auto no_of_channels = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;

        //     tag = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        // }

        // if (tag == 6)
        // {
        //     auto descriptor_length = read_descriptor_length(bs);
        //     auto sl_value = bs.read_uint8_t();
        //     m_remaining_bytes -= 1;
        // }


        bs.skip(m_remaining_bytes);
    }

    std::string esds::describe() const
    {
        std::stringstream ss;
        ss << full_box::describe() << std::endl;
        return ss.str();
    }
}
}
