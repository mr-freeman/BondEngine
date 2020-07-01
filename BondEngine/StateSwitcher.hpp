// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <GamePlay.hpp>
#include <LevelLoad.hpp>
#include <MainMenu.hpp>

#include <IRender.hpp>

#include <GUIConsole.hpp>
#include <FPSCounter.hpp>
#include <FPSCamera.hpp>
#include <Window.hpp>

#include <InputHandler.hpp>

namespace be::game
{
    // Switcher of Game States i.e. MAIN_MENU, LEVEL_LOAD, GAME_PLAY.
    // Main Game cycle to update all the game resources.
    using namespace Diligent;

    class StateSwitcher
    {
    public:
        StateSwitcher();
       ~StateSwitcher() = default;

        StateSwitcher(const StateSwitcher&) = delete;
        StateSwitcher(StateSwitcher&&) = delete;

        StateSwitcher operator=(const StateSwitcher&) = delete;
        StateSwitcher operator=(StateSwitcher&&) = delete;

    public:

        enum class StateType
        {
            MAIN_MENU, // Rendering main menu UI i.e. buttons, text, background image; getting input; playing mm music
            LEVEL_LOAD, // Rendering level pre-image while loading the very level, playing its music
            GAME_PLAY // Rendering actual game, getting input, playing sounds and ambients in the actual game
        };

    public:
        // return false if exit button is pressed, otherwise true
        bool update(const double& current, const double& elapsed, gainput::InputMap& map);
        void render_ui(const double& fps);

    public:
        void setRenderer(shared_ptr<render::IRender>& rend);

    private:
        bool is_menu_opened = true;
        bool is_exit_pressed = false;

        //----------------

        StateType state;

        bool show_console = false;
        bool console_opened = true;
        bool is_game_loaded = false;
        bool is_cursor_shown = true;


        gui::test::ExampleAppConsole console;

        unique_ptr<GamePlay> game_play;
        unique_ptr<LevelLoad> level_load;
        unique_ptr<MainMenu> main_menu;

        unique_ptr<FPSCamera> fps_camera;
    };
}