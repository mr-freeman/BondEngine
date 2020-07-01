// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IGameObject.hpp>

namespace be::game
{
    class Terrain : public IGameObject
    {
    public:
        Terrain() = default;
       ~Terrain() = default;

        Terrain(const Terrain&) = delete;
        Terrain(Terrain&&) = delete;

        Terrain operator=(const Terrain&) = delete;
        Terrain operator=(Terrain&&) = delete;

    public:
        // grass

        // obj
        // texture
    };
}