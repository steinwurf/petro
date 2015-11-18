// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <istream>
#include <iostream>
#include <memory>
#include "parser.hpp"
#include "byte_stream.hpp"
#include "box/unknown.hpp"
#include "box/box.hpp"

namespace petro
{
    template <class... Boxes>
    class parser
    {

    public:

        void read(
            std::vector<box::box*>& boxes,
            const uint8_t* byte_data,
            uint32_t byte_size,
            petro::box::box* parent=nullptr)
        {
            byte_stream bs(byte_data, byte_size);
            while(bs.size() != 0)
            {
                parse(boxes, bs, parent);
            }
        }

        void parse(std::vector<box::box*>& boxes, byte_stream& bs,
            petro::box::box* parent=nullptr)
        {
            // size is an integer that specifies the number of bytes in this
            // box, including all its fields and contained boxes.

            uint32_t size = bs.read_uint32_t();


            // type identifies the box type; standard boxes use a compact type,
            // which is normally four printable characters,
            // to permit ease of identification,
            // and is shown so in the boxes below.
            // User extensions use an extended type; in this case,
            // the type field is set to "uuid".
            std::string type = bs.read_type();


            bool found_box = parse<Boxes...>(boxes, type, size, bs, parent);
            if (not found_box)
                boxes.push_back(new box::unknown(type, size, bs, parent));
        }

    private:

        template<class Box>
        bool parse(std::vector<box::box*>& boxes, const std::string& type,
            uint32_t size, byte_stream& bs,
            petro::box::box* parent)
        {
            if (Box::TYPE == type)
            {
                boxes.push_back(new Box(size, bs, parent));
                return true;
            }
            return false;
        }

        template<class Box, class NextBox, class... Remaining>
        bool parse(std::vector<box::box*>& boxes, const std::string& type,
            uint32_t size, byte_stream& bs,
            petro::box::box* parent)
        {
            return parse<Box>(boxes, type,  size, bs, parent) or
                   parse<NextBox, Remaining...>(boxes, type, size, bs, parent);
        }
    };
}
