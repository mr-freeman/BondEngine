// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <CMDLine.hpp>

namespace be::core::cmd
{
    void CMDLine::initialize(vector<IEngine::Command>& commands)
    {
        using EngineCommand = IEngine::Command;

        EngineCommand exit = { "exit", false }; // global: execute exit

        EngineCommand clear = { "clear", false }; // console: clear console history
        EngineCommand help = { "help", false }; // console: show console history
        EngineCommand info = { "info", false }; // console: show engine info

        EngineCommand fps_info = { "fps_info", false }; // text: show fps info
        EngineCommand cpu_info = { "cpu_info", false }; // text: show cpu info (name, threads, usage)
        EngineCommand camera_info = { "camera_info", false }; // text: shows camera info (position, look_at vector)

        EngineCommand god = { "god", false }; // game: allow free_camera command, actor inevitability
        EngineCommand free_camera = { "free_camera", false }; // game: fly camera
        EngineCommand save = { "save", false }; // game: save
        EngineCommand load = { "load", false }; // game: load save
        EngineCommand fov = { "fov", 75.f }; // game: changes field of view

        commands = { exit,
                     clear, help, info,
                     fps_info, cpu_info, camera_info,
                     god, free_camera, save, load, fov };
    }
}