// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <istream>
#include <memory>
#include <iostream>

#include "byte_stream.hpp"
#include "box/unknown.hpp"
#include "box/box.hpp"
#include "box/root.hpp"

namespace petro
{
    template <class... PBoxes>
    class parser
    {
    public:

        std::shared_ptr<box::box> read(
            byte_stream& bs,
            std::weak_ptr<box::box> parent = std::make_shared<box::root>())
        {
            while (bs.remaining_bytes() != 0)
            {
                parse(bs, parent);
            }
            return parent.lock();
        }

        void parse(byte_stream& bs, std::weak_ptr<box::box> parent)
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
            auto box = parse_helper<PBoxes...>::call(type, size, bs, parent);
            assert(box);
            auto p = parent.lock();
            assert(p);
            p->add_child(box);
        }

    private:

        template<class... Boxes>
        struct parse_helper
        {
            static std::shared_ptr<box::box> call(
                const std::string& type,
                uint32_t size,
                byte_stream& bs,
                std::weak_ptr<box::box> parent)
            {
                // if the parser doesn't support the given type, a generic box
                // type called "unknown" is used instead.
                auto box = std::make_shared<box::unknown>(type, parent);
                box->read(size, bs);
                return box;
            }
        };

        template<class Box, class... Boxes>
        struct parse_helper<Box, Boxes...>
        {
            static std::shared_ptr<box::box> call(
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
                return parse_helper<Boxes...>::call(type, size, bs, parent);
            }
        };
    };
}
