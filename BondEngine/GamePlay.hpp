// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <SimpleCube.hpp>

namespace be::game
{
    // One of three major game states.
    // Actual playing game by player, can be entered with buttons
    // "New Game"/"Load Game" through LevelLoad
    class GamePlay
    {
    public:
        GamePlay();
       ~GamePlay() = default;

        GamePlay(const GamePlay&) = delete;
        GamePlay(GamePlay&&) = delete;

        GamePlay operator=(const GamePlay&) = delete;
        GamePlay operator=(GamePlay&&) = delete;

    public:
        void initialize(shared_ptr<render::IRender>& rend);
        void render_ui(const math::float3& camera_position);
        void render(math::float4x4& wvp);
        void update(math::float4x4& wvp, const float& current);

    private:
        unique_ptr<SimpleCube> simple_cube;
    };
}