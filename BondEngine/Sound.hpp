// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be
{
    class Sound
    {
    public:
        Sound() = default;
       ~Sound() = default;

        Sound(const Sound&) = delete;
        Sound(Sound&&) = delete;

        Sound operator=(const Sound&) = delete;
        Sound operator=(Sound&&) = delete;
    };
}