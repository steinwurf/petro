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

    std::error_code error;
    stack.open(error);
    ASSERT_FALSE(bool(error));

    EXPECT_NE(nullptr, stack.data());
    EXPECT_EQ(711515U, stack.data_size());
    stack.close();
}

TEST(extractor_test_memory_mapped_file_layer, fail)
{
    auto test_filename = "none_existing.mp4";
    petro::extractor::memory_mapped_file_layer stack;
    stack.set_file_path(test_filename);
    EXPECT_EQ(test_filename, stack.file_path());
    std::error_code error;
    stack.open(error);
    EXPECT_TRUE(bool(error));
}
