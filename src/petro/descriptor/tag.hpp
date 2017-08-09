// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

namespace petro
{
namespace descriptor
{
enum class tag : uint8_t
{
    elemetary_stream = 0x03,
    decoder_config = 0x04,
    decoder_specific_info = 0x05,
    sync_layer_config = 0x06,
    content_id = 0x07,
    ip_dentification_data_set = 0x09,
    ipmp = 0x0A,
    qos = 0x0C,
    registration = 0x0D,
    language = 0x43,
    extension = 0x80
};
}
}
