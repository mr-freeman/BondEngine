// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::utils
{
    class Date
    {
    public:
        Date(uint day, uint month, uint year) :
            day(day),
            month(month),
            year(year)
        { }

        Date() :
            year(1),
            month(1),
            day(1)
        { }

       ~Date() = default;

        Date(const Date& date)
        {
            this->day = date.day;
            this->month = date.month;
            this->year = date.year;
        }

        Date(Date&& date) noexcept
        {
            Date buffer;

            buffer.day = date.day;
            buffer.month = date.month;
            buffer.year = date.year;

            this->day = buffer.day;
            this->month = buffer.month;
            this->year = buffer.year;
        }

    public:
        uint year,
             month,
             day;
    };
}