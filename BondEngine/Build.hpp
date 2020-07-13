// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <Date.hpp>

namespace be::utils
{
    class Build
    {
    public:
        Build() = default;
       ~Build() = default;

        Build(const Build&) = delete;
        Build(Build&&) = delete;

        Build operator=(const Build&) = delete;
        Build operator=(Build&&) = delete;

    public:
        uint build_id(const Date& build_date, const string& current_date);
        uint calculate_build_id(string current_date);

    private:
        unsigned int id;

        Date start;
        Date build;
    };
}