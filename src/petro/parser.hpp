// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <istream>
#include <memory>

#include <endian/stream_reader.hpp>
#include <endian/big_endian.hpp>

#include "byte_stream.hpp"
#include "helper.hpp"
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
        std::string type = helper::type(bs.read_uint32_t());

        // Try parsing the found type
        auto box = parse_helper<PBoxes...>::call(type, size, bs, parent);
        assert(box);
        auto p = parent.lock();
        assert(p);
        p->add_child(box);
    }

    std::shared_ptr<box::box> read3(
        const uint8_t* data, uint64_t size,
        std::weak_ptr<box::box> parent,
        std::error_code& error)
    {
        assert(!error);
        uint64_t position = 0;
        while (position != size)
        {
            auto box_size = parse3(data + position, size - position, parent, error);
            std::cout << "box size: " << box_size << std::endl;
            if (error)
                return nullptr;
            position += box_size;
            if (position > size)
            {
                error = std::make_error_code(std::errc::value_too_large);
                return nullptr;
            }
        }
        return parent.lock();
    }

    uint64_t parse3(const uint8_t* data, uint64_t size,
                std::weak_ptr<box::box> parent,
                std::error_code& error)
    {
        assert(!error);

        endian::stream_reader<endian::big_endian> bs(data, size);
        // Skip the size, we are only interested in the type
        bs.skip(4);
        if (error)
            return 0;

        // type identifies the box type; standard boxes use a compact type,
        // which is normally four printable characters,
        // to permit ease of identification.
        // User extensions use an extended type; in this case,
        // the type field is set to "uuid".
        uint32_t type_value;
        bs.read(type_value, error);
        if (error)
            return 0;
        std::string type = helper::type(type_value);

        // Try parsing the found type
        auto box = parse_helper3<PBoxes...>::call(type, data, size, error);
        if (error)
            return 0;

        assert(box);
        auto p = parent.lock();
        assert(p);
        p->add_child(box);
        box->set_parent(p);
        return box->size();
    }

private:

    template<class...>
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

    template<class...>
    struct parse_helper3
    {
        static std::shared_ptr<box::box> call(
            const std::string& type,
            const uint8_t* data,
            uint64_t size,
            std::error_code& error)
        {
            (void) type;
            // if the parser doesn't support the given type, a generic box
            // type called "unknown" is used instead.
            auto box = std::make_shared<box::unknown>(data, size);
            box->parse(error);
            return box;
        }
    };

    template<class Box, class... Boxes>
    struct parse_helper3<Box, Boxes...>
    {
        static std::shared_ptr<box::box> call(
            const std::string& type,
            const uint8_t* data,
            uint64_t size,
            std::error_code& error)
        {
            if (Box::TYPE == type)
            {
                auto box = std::make_shared<Box>(data, size);
                box->parse(error);
                return box;
            }
            return parse_helper3<Boxes...>::call(type, data, size, error);
        }
    };
};
}
