// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <istream>
#include <memory>
#include <cstring>

#include <endian/stream_reader.hpp>
#include <endian/big_endian.hpp>

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

    std::shared_ptr<box::base_box> parse(
        const uint8_t* data,
        uint64_t size,
        std::weak_ptr<box::base_box> parent,
        std::error_code& error)
    {
        assert(!error);
        uint64_t position = 0;
        while (position != size)
        {
            auto box = parse_box(data + position, size - position, parent, error);
            if (error)
                return nullptr;
            position += box->size();
            if (position > size)
            {
                error = std::make_error_code(std::errc::value_too_large);
                return nullptr;
            }
        }
        return parent.lock();
    }

    std::shared_ptr<box::box> parse_box(
        const uint8_t* data,
        uint64_t size,
        std::weak_ptr<box::base_box> parent,
        std::error_code& error)
    {
        assert(!error);

        stream_error_code_wrapper bs(data, size);
        // Skip the size, we are only interested in the type
        bs.skip(4, error);
        if (error)
            return 0;

        // type identifies the box type; standard boxes use a compact type,
        // which is normally four printable characters,
        // to permit ease of identification.
        // User extensions use an extended type; in this case,
        // the type field is set to "uuid".
        std::string type;
        bs.read_type(type, error);
        if (error)
            return 0;

        // Try parsing the found type
        auto box = parse_helper<PBoxes...>::call(type, data, size, parent, error);
        if (error)
            return 0;
        assert(box);

        auto p = parent.lock();
        assert(p);
        p->add_child(box);

        return box;
    }

private:

    template<class...>
    struct parse_helper
    {
        static std::shared_ptr<box::box> call(
            const std::string& type,
            const uint8_t* data,
            uint64_t size,
            std::weak_ptr<box::base_box> parent,
            std::error_code& error)
        {
            (void) type;
            // if the parser doesn't support the given type, a generic box
            // type called "unknown" is used instead.
            auto box = std::make_shared<box::unknown>(data, size);

            box->set_parent(parent);

            box->parse(error);
            return box;
        }
    };

    template<class Box, class... Boxes>
    struct parse_helper<Box, Boxes...>
    {
        static std::shared_ptr<box::box> call(
            const std::string& type,
            const uint8_t* data,
            uint64_t size,
            std::weak_ptr<box::base_box> parent,
            std::error_code& error)
        {
            if (Box::TYPE == type)
            {
                auto box = std::make_shared<Box>(data, size);

                box->set_parent(parent);

                box->parse(error);
                return box;
            }
            return parse_helper<Boxes...>::call(type, data, size, parent, error);
        }
    };
};
}
