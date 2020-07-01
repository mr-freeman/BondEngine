// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <Sound.hpp>
#include <Sound2D.hpp>

#include <filesystem>

namespace be::sound
{
    class Sound3D : public Sound, public Sound2D
    {
    public:
        Sound3D() = default;
       ~Sound3D() = default;

        Sound3D(const Sound3D&) = delete;
        Sound3D(Sound3D&&) = delete;

        Sound3D operator=(const Sound3D&) = delete;
        Sound3D operator=(Sound3D&&) = delete;

    public:
        // position
    };
}