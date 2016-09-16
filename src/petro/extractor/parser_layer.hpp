// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <memory>

#include "../parser.hpp"
#include "../box/moov.hpp"
#include "../box/trak.hpp"
#include "../box/mdia.hpp"
#include "../box/hdlr.hpp"
#include "../box/mdhd.hpp"
#include "../box/minf.hpp"
#include "../box/stbl.hpp"
#include "../box/stco.hpp"
#include "../box/stsd.hpp"
#include "../box/co64.hpp"
#include "../box/stsz.hpp"
#include "../box/stsc.hpp"
#include "../box/ctts.hpp"
#include "../box/stts.hpp"
#include "../box/stsz.hpp"
#include "../box/stbl.hpp"
#include "../box/root.hpp"

namespace petro
{
namespace extractor
{
    /// This layer exposes the "root" of the tree constructed using the mp4 box
    /// parser.
    template<class Super>
    class parser_layer : public Super
    {
    public:

        /// Open this and the underlying layer, returns false upon failure.
        bool open()
        {
            if (!Super::open())
            {
                Super::close();
                return false;
            }

            parser<
                box::moov<parser<
                    box::trak<parser<
                        box::mdia<parser<
                            box::hdlr,
                            box::mdhd,
                            box::minf<parser<
                                box::stbl<parser<
                                    box::stco,
                                    box::stsc,
                                    box::stsd,
                                    box::co64,
                                    box::ctts,
                                    box::stts,
                                    box::stsz
                                >>
                            >>
                        >>
                    >>
                >>
            > parser;

            byte_stream bs(Super::data(), Super::data_size());

            m_root = parser.read(bs);
            return true;
        }

        /// Close this and the underlying layer.
        void close()
        {
            Super::close();
            m_root.reset();
        }

        /// Return a shared pointer to the root box
        std::shared_ptr<box::box> root() const
        {
            assert(m_root != nullptr);
            return m_root;
        }

    private:

        std::shared_ptr<box::box> m_root;

    };
}
}
