// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <ICreature.hpp>

#include <FPSCamera.hpp>
#include <Sound2D.hpp>

namespace be::game
{
    class Actor : public ICreature
    {
    public:
        Actor() = default;
       ~Actor() = default;

        Actor(const Actor&) = delete;
        Actor(Actor&&) = delete;

        Actor operator=(const Actor&) = delete;
        Actor operator=(Actor&&) = delete;

    public:
        enum class State
        {
            ALIVE,
            DEAD
        };

        enum class AliveState
        {
            STAY,
            WALK,
            SPRINT,
            JUMP
        };

    public:
        void updateStats()
        {
            if (health > 100)
                health = 100;

            if (health < 0)
                health = 0;

            if (stamina > 100)
                stamina = 100;

            if (stamina < 0)
                stamina = 0; // Ctrl+C, Ctrl+V ;)
        };

    private:
        // Mesh
        unique_ptr<FPSCamera> camera;

        shared_ptr<sound::Sound2D> ui; // inventory etc
        shared_ptr<sound::Sound2D> voice; // player hear it literally in fps, so let it be 2D
        shared_ptr<sound::Sound2D> booster;
        shared_ptr<sound::Sound2D> notification; // missions, messages etc

        State state;

        // Stats.
        string name;
        uint health;
        uint stamina;

        bool is_armour_worn; // not sure, it's needed program
        uint armour;
    };
}