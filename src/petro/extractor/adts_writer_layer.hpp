// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

namespace petro
{
namespace extractor
{
    template<class Super>
    class adts_writer_layer : public Super
    {
    public:

        /// Uses values from the underlying layers to construct and write an
        /// adts header to the given pointer.
        void write_adts_header(uint8_t* data) const
        {
            // unchangeable
            uint8_t number_of_raw_data_blocks = 1;
            uint8_t mpeg_version = 0;
            uint8_t protection_absent = 1;

            auto channel_configuration = Super::channel_configuration();
            auto mpeg_audio_object_type = Super::mpeg_audio_object_type();
            auto frequency_index = Super::frequency_index();
            auto sample_size = Super::sample_size();

            uint8_t byte1 = 0xFF;
            data[0] = byte1;

            uint8_t byte2 = 0xF0;
            byte2 |= static_cast<uint8_t>(mpeg_version) << 3;
            byte2 |= protection_absent << 0;
            data[1] = byte2;

            uint8_t byte3 = 0x00;
            byte3 |= (mpeg_audio_object_type - 1) << 6;
            byte3 |= (frequency_index) << 2;
            byte3 |= (channel_configuration & 0x04) >> 2;
            data[2] = byte3;

            uint8_t byte4 = 0;
            byte4 |= (channel_configuration & 0x03) << 6;
            // frame length, this value must include the 7 bytes of header length
            uint16_t frame_length = sample_size + 7;
            assert(frame_length <= 0x1FFF);
            byte4 |= (frame_length & 0x1800) >> 11;
            data[3] = byte4;

            uint8_t byte5 = (frame_length & 0x07F8) >> 3;
            data[4] = byte5;

            uint8_t byte6 = 0xFF;
            byte6 &= (frame_length & 0x0007) << 5;
            data[5] = byte6;

            uint8_t byte7 = 0xB0;
            byte7 |= (number_of_raw_data_blocks - 1) & 0x03;
            data[6] = byte7;
        }

        /// returns the size of the header.
        uint32_t adts_header_size() const
        {
            return 7U;
        }
    };
}
}
