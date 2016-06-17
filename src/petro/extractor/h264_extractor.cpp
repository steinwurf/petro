// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "h264_extractor.hpp"

namespace petro
{
    h264_extractor::h264_extractor(std::ifstream& file) :
        m_file(file)
    {

    }


    // Private methods
    bool h264_extractor::has_next_nalu()
    {

        if(m_chunk < m_chunk_offsets.size())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void h264_extractor::setup()
    {
        auto root = std::make_shared<box::root>();
        byte_stream bs(m_file);

        m_parser.read(root, bs);

        //get needed boxes
        m_avc1 = root->get_child("avc1");
        assert(m_avc1 != nullptr);

        m_avcc = m_avc1->get_child<box::avcc>();
        assert(m_avcc != nullptr);

        m_trak = m_avc1->get_parent("trak");
        assert(m_trak != nullptr);

        m_stco = m_trak.get_child<box::stco>();
        if (m_stco != nullptr)
        {
            m_chunk_offsets.resize(m_stco->entry_count());
            std::copy(
                stco->entries().begin(),
                stco->entries().end,
                m_chunk_offsets.begin());
        }
        else
        {
            auto co64 = trak->get_child<box::co64>();
            assert(co64 != nullptr);
            chunk_offsets.resize(co64->entry_count());
            std::copy(
                co64->entries().begin(),
                co64->entries().end(),
                chunk_offsets.begin());
        }

        m_stsc = m_trak->get_child<box::stsc>();
        assert(m_stsc != nullptr);

        m_stsz = m_trak->get_child<box::stsz>();
        assert(m_stsz != nullptr);
    }

    std::vector<char> h264_extractor::next_nalu()
    {

    }
}
