// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "helper.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace petro
{
std::string helper::type(uint32_t data)
{
    uint8_t* data_ptr = (uint8_t*)(&data);
    char c1 = data_ptr[3];
    char c2 = data_ptr[2];
    char c3 = data_ptr[1];
    char c4 = data_ptr[0];
    return {c1, c2, c3, c4};
}

double helper::fixed_point_1616(uint32_t data)
{
    return ((double) data) / (1 << 16);
}


double helper::fixed_point_0230(uint32_t data)
{
    return ((double) data) / (1 << 30);
}

float helper::fixed_point_88(uint32_t data)
{
    return ((float) data) / (1 << 8);
}

std::string helper::iso639(uint16_t data)
{
    char c1 = 0x60 + ((data & 0x7C00) >> 10);  // Mask is 0111 1100 0000 0000
    char c2 = 0x60 + ((data & 0x03E0) >> 5);   // Mask is 0000 0011 1110 0000
    char c3 = 0x60 + ((data & 0x001F));        // Mask is 0000 0000 0001 1111
    return {c1, c2, c3};
}

std::string helper::time32(uint32_t data)
{
    return time(data);
}

std::string helper::time64(uint64_t data)
{
    return time(data);
}

std::string helper::time(uint64_t data)
{
    // mp4 time  is the seconds since 00:00, Jan 1 1904 UTC
    // time_t    is the seconds since 00:00, Jan 1 1970 UTC
    // seconds between 01/01/1904 00:00:00 and 01/01/1970 00:00:00
    uint64_t seconds_between_1904_and_1970 = 2082844800;

    // handle the limited time representation of time_t.
    if (data < seconds_between_1904_and_1970)
    {
        return "before 1970-01-01 00:00:00";
    }

    std::time_t t = data - seconds_between_1904_and_1970;
    // Convert time_t to tm as UTC time
    auto utc_time = std::gmtime(&t);
    char buffer[20];
    std::strftime(buffer, 20, "%F %T", utc_time);
    return std::string(buffer);
}
}
