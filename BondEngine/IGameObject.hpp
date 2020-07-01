// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::game
{
    class IGameObject
    {
    public:
        IGameObject() = default;
       ~IGameObject() = default;

        IGameObject(const IGameObject&) = default;
        IGameObject(IGameObject&&) = default;

        IGameObject operator=(const IGameObject&) = delete;
        IGameObject operator=(IGameObject&&) = delete;

    public:

    protected:
        unsigned int count = 0;
    };
}