// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <MainMenu.hpp>

namespace be::game
{
    void MainMenu::render_ui()
    {
        ImGui::SetNextWindowPos({ 80, 250 }, ImGuiCond_::ImGuiCond_Once);
        ImGui::Begin("Main Menu", (bool*)false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

        new_game_pressed = ImGui::Button("New Game", { 90, button_size });
        load_game_pressed = ImGui::Button("Load Game", { 90, button_size });
        settings_pressed = ImGui::Button("Settings", { 90, button_size });
        exit_pressed = ImGui::Button("Exit", { 90, button_size });

        ImGui::End();

        if (exit_pressed)
            ep = true;

        //------------
        unsigned long long x = (1366 / 2);
        auto countPercentage = [](unsigned long long input, int percentage) -> long long
        {
            long long x = static_cast<long long>(input) * static_cast<long long>(percentage);
            long long ret = x / 100;

            if (x - ret * 100 >= 50)
                ++ret;

            return static_cast<int>(ret);
        };

        x = countPercentage(x, 82);
        auto y = (768 / 2);
        //------------

        ImVec2 window_center = { static_cast<float>(x), static_cast<float>(y) };

        if (ep)
        {
            ImGui::SetNextWindowPos(window_center, ImGuiCond_Once);
            ImGui::Begin("Make Sure Window", (bool*)true, ImGuiWindowFlags_MenuBar |
                         ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoMove);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Are you sure you want to quit?", true))
                {
                    if (ImGui::MenuItem("No"))
                        ep = false;

                    // Doesn't solve offset problem for clicking.
                    //ImGui::SameLine(0.f, (10.f));
                    if (ImGui::MenuItem("Yes"))
                        execute_exit = true;

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            
            ImGui::End();
        }
    };
}