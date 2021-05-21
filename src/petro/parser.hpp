// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstring>
#include <istream>
#include <memory>
#include <string>

#include "box/data_box.hpp"
#include "box/root.hpp"
#include "box/unknown.hpp"
#include "stream.hpp"

namespace petro
{
template <class... PBoxes>
class parser
{
public:
    void parse(const uint8_t* data, uint64_t size,
               std::weak_ptr<box::box> parent, std::error_code& error)
    {
        assert(data != nullptr);
        assert(size != 0);
        assert(parent.lock() != nullptr);
        assert(!error);

        uint64_t position = 0;
        while (position != size)
        {
            auto box =
                parse_box(data + position, size - position, parent, error);
            if (error)
                return;
            position += box->size();
            assert(position <= size &&
                   "This should not be possible, the boxes should set the "
                   "error code if they read more than size.");
        }
    }

    std::shared_ptr<box::data_box> parse_box(const uint8_t* data, uint64_t size,
                                             std::weak_ptr<box::box> parent,
                                             std::error_code& error)
    {
        assert(data != nullptr);
        assert(size != 0);
        assert(parent.lock() != nullptr);
        assert(!error);

        stream bs(data, size);
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
        auto box =
            parse_helper<PBoxes...>::call(type, data, size, parent, error);
        if (error)
            return 0;
        assert(box);

        auto p = parent.lock();
        assert(p);
        p->add_child(box);

        return box;
    }

private:
    template <class...>
    struct parse_helper
    {
        static std::shared_ptr<box::data_box>
        call(const std::string& type, const uint8_t* data, uint64_t size,
             std::weak_ptr<box::box> parent, std::error_code& error)
        {
            (void)type;
            // if the parser doesn't support the given type, a generic box
            // type called "unknown" is used instead.
            auto box = std::make_shared<box::unknown>(data, size);

            box->set_parent(parent);

            box->parse(error);
            return box;
        }
    };

    template <class Box, class... Boxes>
    struct parse_helper<Box, Boxes...>
    {
        static std::shared_ptr<box::data_box>
        call(const std::string& type, const uint8_t* data, uint64_t size,
             std::weak_ptr<box::box> parent, std::error_code& error)
        {
            if (Box::TYPE == type)
            {
                auto box = std::make_shared<Box>(data, size);

                box->set_parent(parent);

                box->parse(error);
                return box;
            }
            return parse_helper<Boxes...>::call(type, data, size, parent,
                                                error);
        }
    };
};
}
