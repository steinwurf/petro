// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/looper.hpp>

#include <stub/function.hpp>
#include <gtest/gtest.h>

namespace
{
    struct dummy_extractor
    {
        stub::function<uint64_t()> timestamp;
        stub::function<void()> advance;
        stub::function<void()> reset;
        stub::function<bool()> at_end;
    };

    using extractor_stack = petro::extractor::looper<dummy_extractor>;
}

TEST(test_looper, not_looping)
{
    extractor_stack extractor;
    dummy_extractor& layer = extractor;
    extractor.disable_looping();
    layer.timestamp.set_return(1);
    layer.at_end.set_return(false, false, true);
    extractor.advance();
    extractor.advance();
    extractor.advance();
    EXPECT_EQ(0U, layer.reset.calls());
    EXPECT_EQ(1U, extractor.timestamp());
    EXPECT_EQ(3U, layer.advance.calls());
}

TEST(test_looper, looping)
{
    extractor_stack extractor;
    dummy_extractor& layer = extractor;
    layer.timestamp.set_return(1);
    layer.at_end.set_return(false, false, true, false, false, true);
    extractor.advance();
    extractor.advance();
    extractor.advance();
    EXPECT_EQ(1U, layer.reset.calls());
    EXPECT_EQ(2U, extractor.timestamp());
    EXPECT_EQ(3U, layer.advance.calls());
    extractor.advance();
    extractor.advance();
    extractor.advance();
    EXPECT_EQ(2U, layer.reset.calls());
    EXPECT_EQ(3U, extractor.timestamp());
    EXPECT_EQ(6U, layer.advance.calls());
}
