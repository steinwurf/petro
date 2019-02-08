// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <petro/extractor/file.hpp>

#include <stub/function.hpp>

#include <gtest/gtest.h>

TEST(extractor_test_file, init)
{
    petro::extractor::file file;
    std::error_code error;
    file.open("test1.mp4", error);
    ASSERT_FALSE(bool(error));

    EXPECT_NE(nullptr, file.data());
    EXPECT_EQ(711515U, file.size());
    file.close();
}

TEST(extractor_test_file, fail)
{
    auto test_filename = "none_existing.mp4";
    std::error_code error;
    petro::extractor::file file;
    file.open(test_filename, error);
    EXPECT_TRUE(bool(error));
}
