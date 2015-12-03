// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include "byte_stream.hpp"

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
            a = bs.read_fixed_point_1616();
            b = bs.read_fixed_point_1616();
            u = bs.read_fixed_point_0230();
            c = bs.read_fixed_point_1616();
            d = bs.read_fixed_point_1616();
            v = bs.read_fixed_point_0230();
            x = bs.read_fixed_point_1616();
            y = bs.read_fixed_point_1616();
            w = bs.read_fixed_point_0230();
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
