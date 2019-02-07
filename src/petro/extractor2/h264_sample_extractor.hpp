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
#include "../box/mvhd.hpp"
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

#include "track_type.hpp"

namespace petro
{
namespace extractor
{
class h264_sample_extractor
{
public:

    void open(const uint8_t* data, uint64_t size, uint32_t track_id, std::error_code& error)
    {
        parser<
            box::moov<parser<
                box::mvhd,
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
        auto root = std::make_shared<petro::box::root>();
        parser.parse(data, size, root, error);
        if (!error)
            m_root = root;
    }

    void close()
    {
        m_root = nullptr;
    }

private:

    std::shared_ptr<petro::box::root> m_root;
};
}
}
