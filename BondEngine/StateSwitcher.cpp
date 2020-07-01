// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include "StateSwitcher.hpp"

#include <FileSystem.hpp>
#include <GUISystem.hpp>
#include <Log.hpp>


#include <gainput/gainput.h>


namespace be::game
{
    StateSwitcher::StateSwitcher() :
        state{ StateType::MAIN_MENU }, // initial game state.
        main_menu{ make_unique<MainMenu>() }, // first thing players see when running games (probably)
        game_play{ make_unique<GamePlay>() }, // first thing players see when running games (probably)
        fps_camera{ make_unique<FPSCamera>() }, // first thing players see through when running games (apparently)
        is_cursor_shown{ true }
    { }

    void StateSwitcher::setRenderer(shared_ptr<render::IRender>& rend)
    {
        game_play->initialize(rend);
        
        fps_camera->setPosition({ 0.0f, -0.f, -1.f });
        fps_camera->setLookAt({ 6.f, 0.f, 0.f });

    }

    bool StateSwitcher::update(const double& current, const double& elapsed, gainput::InputMap& input_map)
    {
        using SoundState = Emyl::Source::State;

        switch (state)
        {
            case StateType::GAME_PLAY:
            {
                if(is_game_loaded == false)
                    is_game_loaded = true;

                game_play->render(fps_camera->getWVP());
                
                if (!show_console)
                {
                    game_play->update(fps_camera->getWVP(), static_cast<float>(current));
                    fps_camera->update(static_cast<float>(current), static_cast<float>(elapsed), input_map);
                }


                if (main_menu->getSound()->getState() != SoundState::Paused)
                    main_menu->getSound()->pause();

                if (input_map.GetBoolWasDown(input::GameButton::Console))
                {
                    ::SetCursorPos(1366 / 2, 768 / 2);

                    show_console = !show_console;
                }

                if (input_map.GetBoolWasDown(input::GameButton::Menu))
                {
                    if(show_console)
                        show_console = false;

                    state = StateType::MAIN_MENU;
                }

                if (is_exit_pressed)
                    return false;

                break;
            }

            case StateType::LEVEL_LOAD:
            {
                if (main_menu->getSound()->getState() != SoundState::Paused)
                    main_menu->getSound()->pause();

                break;
            }

            case StateType::MAIN_MENU:
            {
                if (main_menu->getSound()->getState() != SoundState::Playing)
                    main_menu->getSound()->play();

                if (main_menu->getNewGamePressed())
                {
                    show_console = false;
                    state = StateType::LEVEL_LOAD;
                }

                if (input_map.GetBoolWasDown(input::GameButton::Menu) && is_game_loaded == true)
                {
                    show_console = false;
                    state = StateType::GAME_PLAY;
                }

                if (input_map.GetBoolWasDown(input::GameButton::Console))
                    show_console = !show_console;

                if (main_menu->getExitPressed() || is_exit_pressed)
                    return false;

                break;
            }
            default:
                state = StateType::MAIN_MENU;
        }

        return true;
    }

    using namespace std::chrono_literals;

    void StateSwitcher::render_ui(const double& fps)
    {
        switch (state)
        {
            case StateType::GAME_PLAY:
            {
                if (is_cursor_shown)
                {
                    ::ShowCursor(FALSE);
                    is_cursor_shown = false;
                }

                game_play->render_ui(fps_camera->getPosition());

                ImGui::Text("%f", fps_camera->GetDeltaX());
                ImGui::Text("%f", fps_camera->GetDeltaY());

                if (show_console)
                {
                    console.Draw("Engine Console");
                    is_exit_pressed = console.exit_command;
                }

                break;
            }

            case StateType::LEVEL_LOAD:
            {
                if (is_cursor_shown)
                {
                    ::ShowCursor(FALSE);
                    is_cursor_shown = false;
                }

                ImGui::Text("Level: %s", "Forsaken Town");
                ImGui::Text("Loading: %d", 99);

                std::chrono::duration imitate_loading(1s);
                std::this_thread::sleep_for(imitate_loading);

                state = StateType::GAME_PLAY;

                break;
            }

            case StateType::MAIN_MENU:
            {
                if (!is_cursor_shown)
                {
                    ::ShowCursor(TRUE);
                    is_cursor_shown = true;
                }

                main_menu->render_ui();

                if (show_console)
                {
                    console.Draw("Engine Console");
                    is_exit_pressed = console.exit_command;
                }

                break;
            }

            default:
                state = StateType::MAIN_MENU;
        }
    }
}