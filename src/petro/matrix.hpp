// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <sstream>

#include "stream.hpp"

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
