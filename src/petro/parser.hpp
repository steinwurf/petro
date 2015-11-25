// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <istream>
#include <memory>
#include <iostream>
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

        void read(std::weak_ptr<box::box> parent, const uint8_t* byte_data,
            uint32_t byte_size)
        {
            byte_stream bs(byte_data, byte_size);
            while(bs.size() != 0)
            {
                parse(parent, bs);
            }
        }

        void parse(std::weak_ptr<box::box> parent, byte_stream& bs)
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

            // Try parsing the found type
            auto found_box = parse_box<Boxes...>(type, size, bs, parent);
            // if the parser doesn't support the given type, a generic box type
            // called "unknown" is used instead.
            if (found_box == nullptr)
            {
                found_box = std::make_shared<box::unknown>(type, parent);
                found_box->read(size, bs);
            }
            auto p = parent.lock();
            assert(p);
            p->add_child(found_box);
        }

    private:

        template<class Box>
        std::shared_ptr<box::box> parse_box(
            const std::string& type,
            uint32_t size,
            byte_stream& bs,
            std::weak_ptr<box::box> parent)
        {
            if (Box::TYPE == type)
            {
                auto box = std::make_shared<Box>(parent);
                box->read(size, bs);
                return box;
            }
            return nullptr;
        }

        template<class Box, class NextBox, class... Remaining>
        std::shared_ptr<box::box> parse_box(
            const std::string& type,
            uint32_t size,
            byte_stream& bs,
            std::weak_ptr<box::box> parent)
        {
            auto box = parse_box<Box>(type,  size, bs, parent);
            if (box != nullptr)
            {
                return box;
            }
            return parse_box<NextBox, Remaining...>(type, size, bs, parent);
        }
    };
}
