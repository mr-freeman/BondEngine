// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <random>

namespace be
{
    // RNG is quite usefull feature in be::game to make random choises.
    class RandomNumberGenerator
    {
    public:
        RandomNumberGenerator() :
            rng(rd())
        { }

    public:
        template<typename T>
        bool generate(const int& min = std::numeric_limits<T>::min(),
                      const int& max = std::numeric_limits<T>::max()) const
        {
            std::uniform_int_distribution<std::mt19937_64::result_type> uid(min, max);

            return uid(rd);
        }

    private:
        std::random_device rd;
        std::mt19937_64 rng;
    };
}