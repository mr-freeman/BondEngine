// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IGameObject.hpp>

#include <Sound2D.hpp>
#include <Terrain.hpp>
#include <SkyBox.hpp>

namespace be::game
{
    class Level : public IGameObject
    {
    public:
        Level()
        {
            ambients.top()->play();
        };
       ~Level()
        {
            ambients.top()->stop();
        }

        Level(const Level&) = delete;
        Level(Level&&) = delete;

        Level operator=(const Level&) = delete;
        Level operator=(Level&&) = delete;

    public:
        void updateAmbient()
        {
            // abmient finished, so
            if (ambients.top()->getState() != Emyl::Sound::State::Playing)
            {
                ambients.pop();
                ambients.top()->play();
            }
        }

        // To update SkyBox we need Environment
        // liked to it, to sync fog, ambient, sun etc

    private:
        string name;
        string ingame_name;
        // preloaded screen
        // ambient list
        // object list
        std::stack<shared_ptr<SkyBox>> skyboxes;
        std::stack<shared_ptr<sound::Sound2D>> ambients;
        shared_ptr<Terrain> terrain;
    };
}