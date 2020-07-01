// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IGameObject.hpp>

namespace be
{
    namespace game
    {
        class ICreature : public IGameObject
        {
        public:
            ICreature() = default;
           ~ICreature() = default;

            ICreature(const ICreature&) = delete;
            ICreature(ICreature&&) = delete;

            ICreature operator=(const ICreature&) = delete;
            ICreature operator=(ICreature&&) = delete;

        public:
            void updateStats() { };

        private:
            // Name, description
            // Mesh e.i. shader, texture
            // Voice, action sounds
            // Sound sources i.e. depth, hello etc
            unsigned int health;
            unsigned int stamina;
        };
    }
}