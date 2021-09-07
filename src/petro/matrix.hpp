// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <sstream>

namespace petro
{
/// specify a transformation of video images for presentation
/// the matrix looks as follows:
///
/// | a b u |
///
/// | c d v |
///
/// | x y w |
///
struct matrix
{
    /// @return a string visualizing the matrix
    std::string describe() const
    {
        std::stringstream ss;
        ss << "|a: " << a << " b: " << b << " u: " << u << "|" << std::endl;
        ss << "|c: " << c << " d: " << d << " v: " << v << "|" << std::endl;
        ss << "|x: " << x << " y: " << y << " w: " << w << "|" << std::endl;
        return ss.str();
    }

    /// entry 1,1
    double a;
    /// entry 1,2
    double b;
    /// entry 1,3
    double u;
    /// entry 2,1
    double c;
    /// entry 2,2
    double d;
    /// entry 2,3
    double v;
    /// entry 3,1
    double x;
    /// entry 3,2
    double y;
    /// entry 3,3
    double w;
};
}
