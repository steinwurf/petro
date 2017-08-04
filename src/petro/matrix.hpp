// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "byte_stream.hpp"
#include "helper.hpp"

namespace petro
{
/// specify a transformation of video images for presentation
/// the matix looks as follows:
///
/// | a b u |
/// | c d v |
/// | x y w |
///
struct matrix
{
    void read(byte_stream& bs)
    {
        a = helper::fixed_point_1616(bs.read_uint32_t());
        b = helper::fixed_point_1616(bs.read_uint32_t());
        u = helper::fixed_point_0230(bs.read_uint32_t());
        c = helper::fixed_point_1616(bs.read_uint32_t());
        d = helper::fixed_point_1616(bs.read_uint32_t());
        v = helper::fixed_point_0230(bs.read_uint32_t());
        x = helper::fixed_point_1616(bs.read_uint32_t());
        y = helper::fixed_point_1616(bs.read_uint32_t());
        w = helper::fixed_point_0230(bs.read_uint32_t());
        // read 9 * 4 (36) bytes
    }

    std::string describe() const
    {
        std::stringstream ss;
        ss << "|a: " << a << " b: " << b << " u: " << u << "|" << std::endl;
        ss << "|c: " << c << " d: " << d << " v: " << v << "|" << std::endl;
        ss << "|x: " << x << " y: " << y << " w: " << w << "|" << std::endl;
        return ss.str();
    }

    double a;
    double b;
    double u;
    double c;
    double d;
    double v;
    double x;
    double y;
    double w;
};
}
