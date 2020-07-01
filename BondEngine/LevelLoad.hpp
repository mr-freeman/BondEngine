// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <Sound2D.hpp>

namespace be::game
{
    // One of three major game states.
    class LevelLoad
    {
    public:
        LevelLoad() = default;
       ~LevelLoad() = default;

        LevelLoad(const LevelLoad&) = delete;
        LevelLoad(LevelLoad&&) = delete;

        LevelLoad operator=(const LevelLoad&) = delete;
        LevelLoad operator=(LevelLoad&&) = delete;

    public:
        void render() { };

    private:
        // texture
        std::unique_ptr<sound::Sound2D> sound;
    };
}