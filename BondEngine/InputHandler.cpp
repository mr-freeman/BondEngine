// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <InputHandler.hpp>

#include <Log.hpp>

namespace be::input
{
    InputHandler::InputHandler() :
        map(manager),
        resolution{0, 0}
    {
        name = "Input Handler"s;
        lib_name = "None"s; // depends on a platform
        
        api_name = "GaInput"s;
        api_version = "1.0.0"s;
    }

    void InputHandler::preinitialize(const math::int2& screen_resolution)
    {
        resolution = screen_resolution;
    }

    void InputHandler::initialize()
    {
        utils::log::info("{} initialization is started", name);
        utils::log::info("{} : {}", api_name, api_version);

        manager.SetDisplaySize(resolution.x, resolution.y);

        keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
        mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();

        // provide variability
        map.MapBool(MoveBackward, keyboardId, gainput::KeyS);
        map.MapBool(MoveForward, keyboardId, gainput::KeyW);
        map.MapBool(MoveRight, keyboardId, gainput::KeyD);
        map.MapBool(MoveLeft, keyboardId, gainput::KeyA);
        map.MapBool(Sprint, keyboardId, gainput::KeyShiftL);
        map.MapBool(Crouch, keyboardId, gainput::KeyCtrlL);

        map.MapBool(Use, keyboardId, gainput::KeyF);

        map.MapBool(LastLoad, keyboardId, gainput::KeyF9);
        map.MapBool(Save, keyboardId, gainput::KeyF5);

        map.MapBool(Console, keyboardId, gainput::KeyExtra5);
        map.MapBool(Menu, keyboardId, gainput::KeyEscape);

        map.MapFloat(MouseX, mouseId, gainput::MouseAxisX);
        map.MapFloat(MouseY, mouseId, gainput::MouseAxisY);


        state = State::INITIALIZED;
        utils::log::info("{} is initialized", name);
    }

    void InputHandler::run()
    {
        manager.Update();
    }

    void InputHandler::finalize()
    {
        if (state != State::INITIALIZED)
            return;

        map.Clear();

        state = State::FINALIZED;
        utils::log::info("{} is finalized", name);
    }
}