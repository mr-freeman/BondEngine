// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::utils
{
    class Time
    {
    public:
        Time(uint hour, uint minute, uint second) :
            hour(hour),
            minute(minute),
            second(second)
        {}

        Time() :
            second(1),
            minute(1),
            hour(1)
        {}

       ~Time() = default;

        Time(const Time& Time)
        {
            this->hour = Time.hour;
            this->minute = Time.minute;
            this->second = Time.second;
        }

        Time(Time&& time) noexcept
        {
            Time buffer;

            buffer.hour = time.hour;
            buffer.minute = time.minute;
            buffer.second = time.second;

            this->hour = buffer.hour;
            this->minute = buffer.minute;
            this->second = buffer.second;
        }

    public:
        uint hour,
            minute,
            second;
    };
}