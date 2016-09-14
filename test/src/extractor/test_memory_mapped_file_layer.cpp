// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/memory_mapped_file_layer.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

TEST(extractor_test_memory_mapped_file_layer, init)
{
    petro::extractor::memory_mapped_file_layer stack;
    stack.set_file_path("test1.mp4");
    EXPECT_EQ("test1.mp4", stack.file_path());
    EXPECT_TRUE(stack.open());
    EXPECT_NE(nullptr, stack.data());
    EXPECT_EQ(711515U, stack.data_size());
    stack.close();
}
