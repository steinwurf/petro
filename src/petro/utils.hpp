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
    std::vector<box::box*> find_boxes(const std::vector<box::box*>& boxes,
        const std::string& type)
    {
        std::vector<box::box*> result;
        for(auto box : boxes)
        {
            if (box->type() == type)
                result.push_back(box);

            auto found_boxes = find_boxes(box->children(), type);
            result.insert(result.end(), found_boxes.begin(), found_boxes.end());
        }
        return result;
    }

    box::box* find_box(
        const std::vector<box::box*>& boxes, const std::string& type)
    {
        for(auto box : boxes)
        {
            if (box->type() == type)
                return box;

            auto found_box = find_box(box->children(), type);
            if (found_box != nullptr)
                return found_box;
        }
        return nullptr;
    }

    box::box* get_ancestor(
        const box::box* b,
        const std::string& grand_parent)
    {
        auto parent = b->parent();
        if (parent != nullptr)
        {
            for (auto child : parent->children())
            {
                if (child->type() == grand_parent)
                {
                    return child;
                }
            }
            if (parent->parent() != nullptr)
            {
                return get_ancestor(parent, grand_parent);
            }
        }
        return nullptr;
    }

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
