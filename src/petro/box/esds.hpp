// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "full_box.hpp"
#include "../byte_stream.hpp"

namespace petro
{
namespace box
{
    /// MPEG-4 Elementary Stream Descriptor
    class esds : public full_box
    {

    public:

        class descriptor
        {
        public:
            descriptor(byte_stream& bs, uint8_t tag):
                m_tag(tag)
            {
                // The next byte is either the descriptor length or the first of
                // a three bytes optional extended descriptor type tag string.
                // The tag types are 0x80,0x81,0xFE.
                m_length = bs.read_uint8_t();
                auto bytes_read = 1;

                if (m_length == 0x80 ||
                    m_length == 0x81 ||
                    m_length == 0xFE)
                {
                    // appearently we are reading the optional extended
                    // descriptor type tag string - let's throw it away...
                    bs.skip(2);
                    bytes_read += 2;
                    // ... and read the descriptor length.
                    m_length = bs.read_uint8_t();
                    bytes_read += 1;
                }

                m_remaining_bytes = m_length - bytes_read;
            }

            uint8_t length()
            {
                return m_length;
            }

            uint32_t remaining_bytes()
            {
                return m_remaining_bytes;
            }

        protected:
            uint8_t m_tag;
            uint8_t m_length;
            uint32_t m_remaining_bytes;
        };

        class elemetary_stream_descriptor : public descriptor
        {
        public:
            elemetary_stream_descriptor(byte_stream& bs, uint8_t tag):
                descriptor(bs, tag)
            {
                assert(m_tag == 3);
                m_es_id = bs.read_uint16_t();
                m_remaining_bytes -= 2;

                auto d = bs.read_uint8_t();
                m_remaining_bytes -= 1;

                auto stream_dependence_flag = (d & 0b00000001) == 1;
                auto url_flag = ((d & 0b00000010) >> 1) == 1;
                auto ocr_stream_flag = ((d & 0b00000100) >> 2) == 1;

                m_byte_stream_priority = (d & 0b11111000) >> 3;

                if (stream_dependence_flag)
                {
                //     m_depends_on_esid = bs.read_uint16_t();
                //     m_remaining_bytes -= 2;
                }

                if (url_flag)
                {
                //     auto url_length = bs.read_uint8_t();
                //     m_remaining_bytes -= 1;

                //     for (uint8_t i = 0; i < url_length; ++i)
                //     {
                //         m_url += bs.read_uint8_t();
                //         m_remaining_bytes -= 1;
                //     }
                }

                if (ocr_stream_flag)
                {
                //     m_ocr_es_id = bs.read_uint16_t();
                //     m_remaining_bytes -= 2;
                }
            }

        private:

            uint16_t m_es_id;
            uint8_t m_byte_stream_priority;
            uint16_t m_depends_on_esid;
            std::string m_url;
            uint16_t m_ocr_es_id;
        };

    public:

        static const std::string TYPE;

    public:
        esds(std::weak_ptr<box> parent);
        void read(uint64_t size, byte_stream& bs);
        std::string describe() const;

    private:

        std::shared_ptr<elemetary_stream_descriptor> m_descriptor;
    };
}
}
