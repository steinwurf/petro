// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <istream>
#include <memory>
#include <cassert>
#include <vector>
#include <fstream>
#include <iostream>

namespace petro
{
// a big endian byte stream used for reading data from an mp4 file.
class helper
{
public:

    static std::string type(uint32_t data);
    static double fixed_point_1616(uint32_t data);
    static double fixed_point_0230(uint32_t data);
    static float fixed_point_88(uint32_t data);
    static std::string iso639(uint16_t data);
    static std::string time32(uint32_t data);
    static std::string time64(uint64_t data);

private:

    static std::string time(uint64_t data);
};
}
