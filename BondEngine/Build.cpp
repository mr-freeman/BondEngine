// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <Build.hpp>

namespace be::utils
{
    uint Build::build_id(const Date& build_date, const string& current_date)
    {
        Date current{ };
        string month_string = ""s;
        std::stringstream buffer;

        buffer << current_date;
        buffer >> month_string;
        buffer >> current.day;
        buffer >> current.year;

        string month_id[12] = 
        {
            "Jan"s,"Feb"s,"Mar"s,"Apr"s,"May"s,"Jun"s,"Jul"s,"Aug"s,"Sep"s,"Oct"s,"Nov"s,"Dec"s
        };

        current.month = 1;
        for (const auto& month : month_id)
            if (month != month_string)
            {
                current.month++;
                continue;
            }
            else
                break;

        auto day_count = [](const Date& date)
        {
            auto const years = date.year;
            auto const extra_day_count = ((years % 400 == 0) || ((years % 4 == 0) && (years % 100 != 0))) ? 1 : 0;
            auto const years_minus_1 = static_cast<uint>(years) - 1;
            auto result = years_minus_1 * 365 + years_minus_1 / 4 - years_minus_1 / 100 + years_minus_1 / 400;

            auto const months = date.month;
            if (months > 1) result += 31;
            if (months > 2) result += 28 + extra_day_count;
            if (months > 3) result += 31;
            if (months > 4) result += 30;
            if (months > 5) result += 31;
            if (months > 6) result += 30;
            if (months > 7) result += 31;
            if (months > 8) result += 31;
            if (months > 9) result += 30;
            if (months > 10) result += 31;
            if (months > 11) result += 30;

            return (result + date.day - 1);
        };

        auto start_days = day_count(build_date);
        auto stop_days = day_count(current);

        return (stop_days - start_days);
    }

    uint Build::calculate_build_id(string current_date)
    {
        // start development date (03.05.2020).
        constexpr auto start_dev_day = 3;
        constexpr auto start_dev_month = 5;
        constexpr auto start_dev_year = 2020;

        return build_id(Date{ start_dev_day, start_dev_month, start_dev_year }, current_date);
    }
}