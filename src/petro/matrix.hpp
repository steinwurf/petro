// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "stream_error_code_wrapper.hpp"

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
    void read(stream_error_code_wrapper& bs, std::error_code& error)
    {
        bs.read_fixed_point_1616(a, error);
        if (error)
            return;

        bs.read_fixed_point_1616(b, error);
        if (error)
            return;

        bs.read_fixed_point_0230(u, error);
        if (error)
            return;

        bs.read_fixed_point_1616(c, error);
        if (error)
            return;

        bs.read_fixed_point_1616(d, error);
        if (error)
            return;

        bs.read_fixed_point_0230(v, error);
        if (error)
            return;

        bs.read_fixed_point_1616(x, error);
        if (error)
            return;

        bs.read_fixed_point_1616(y, error);
        if (error)
            return;

        bs.read_fixed_point_0230(w, error);
        if (error)
            return;
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
