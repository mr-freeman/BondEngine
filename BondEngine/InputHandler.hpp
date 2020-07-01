// Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

namespace be::input
{
    using namespace gainput;

    // gainput automatically manages device connections
    // so there's no need to provice device_is_univailable error code
    class InputHandler : public IEngine, public IEngine::Component
    {
    public:
        InputHandler();
       ~InputHandler() = default;

        InputHandler(const InputHandler&) = delete;
        InputHandler(InputHandler&&) = delete;

        InputHandler& operator=(const InputHandler&) = delete;
        InputHandler& operator=(InputHandler&&) = delete;

    public:
        void preinitialize(const math::int2& screen_resolution);
        void initialize() override;
        void run() override;
        void finalize() override;

        InputMap& getMap() { return map; }
        InputManager& getManager() { return manager; }

    private:
        InputManager manager;
        InputMap map;

        DeviceId mouseId;
        DeviceId keyboardId;

        math::int2 resolution;
    };

    enum GameButton
    {
        MoveBackward,
        MoveForward,
        MoveRight,
        MoveLeft,
        Sprint,
        Crouch,

        Use,

        LastLoad,
        Save,

        Console,
        Menu,

        MouseX,
        MouseY
    };
}