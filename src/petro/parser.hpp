// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <string>
#include <istream>
#include <iostream>
#include <memory>
#include "parser.hpp"
#include "byte_stream.hpp"
#include "box/box.hpp"

namespace petro
{
    template <class... Boxes>
    class parser
    {

    public:

        std::vector<box::box*> read(
            const uint8_t* byte_data, uint32_t byte_size,
            petro::box::box* parent=nullptr)
        {
            byte_stream bs(byte_data, byte_size);
            std::vector<box::box*> boxes;
            while(bs.size() != 0)
            {
                parse(boxes, bs, parent);
            }
            return boxes;
        }

        bool parse(std::vector<box::box*>& boxes, byte_stream& bs,
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

            if (size == 1)
            {
                // if size is 1 then the actual size is in the field
                // largesize;
                size = bs.read_uint64_t();
                size -= 8;
            }
            else if (size == 0)
            {
                // if size is 0, then this box is the last one in the file,
                // and its contents extend to the end of the file
                // (normally only used for a Media Data Box (mdat))
                size = bs.size();
            }


            // the size is included in size, so we need to accommodate
            // for this.
            size -= 4;

            // the type is included in size, so we need to accommodate for
            // this.
            size -= 4;


            if (type == "uuid")
            {
                // unsigned int(8)[16] usertype = extended_type;
                // type = extended_type
                // size -= sizeof(extended_type)
            }
            bool result = parse<Boxes...>(boxes, type, bs, size, parent);
            if (not result)
                bs.skip(size);
            return result;
        }

    private:

        template<class Box>
        bool parse(std::vector<box::box*>& boxes, const std::string& type,
            byte_stream& bs, uint32_t size,
            petro::box::box* parent)
        {
            if (Box::TYPE == type)
            {
                boxes.push_back(new Box(bs, size, parent));
                return true;
            }
            return false;
        }

        template<class Box, class NextBox, class... Remaining>
        bool parse(std::vector<box::box*>& boxes, const std::string& type,
            byte_stream& bs, uint32_t size,
            petro::box::box* parent)
        {
            return parse<Box>(boxes, type, bs, size, parent) or
                   parse<NextBox, Remaining...>(boxes, type, bs, size, parent);
        }
    };
}
