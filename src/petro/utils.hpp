// Copyright Steinwurf ApS 2015.
// All Rights Reserved

#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "box/box.hpp"

namespace petro
{
    std::string parse_time(uint64_t total_time)
    {
        // 2082844800 seconds between 01/01/1904 & 01/01/1970
        // 2081376000 + 1468800 (66 years + 17 leap days)
        total_time -= 2082844800;

        // asctime creates a char* with the following format:
        //     Www Mmm dd hh:mm:ss yyyy\n
        std::time_t t = total_time;
        char* time_chars = std::asctime(std::localtime(&t));

        std::stringstream ss;
        // We don't want the trailing newline so we only pick the first 24
        // chars.
        ss << std::string(time_chars, 24);
        return ss.str();
    }
}
