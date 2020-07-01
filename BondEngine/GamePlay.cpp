// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include "GamePlay.hpp"

namespace be::game
{
    GamePlay::GamePlay() :
        simple_cube{ std::make_unique<SimpleCube>() }
    {
        
    }

    void GamePlay::initialize(shared_ptr<render::IRender>& rend)
    {
        simple_cube->setRenderer(rend);

        simple_cube->initialize();
        simple_cube->loadTexture();
    }

    // Draft
    static inline void renderText(const math::int2& win_pos, const string& win_name, const std::vector<string>& text_lines)
    {
        ImVec2 pos = { static_cast<float>(win_pos.x), static_cast<float>(win_pos.y) };
        ImGui::SetNextWindowPos(pos, ImGuiCond_::ImGuiCond_Once);

        ImGui::Begin(win_name.c_str(), (bool*)false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

        for (const auto i : text_lines)
            ImGui::Text(i.c_str());

        ImGui::End();
    };

    // Draft
    static inline vector<bool> renderButton(const math::int2& win_pos, const string& win_name, const vector<string>& buttons)
    {
        float button_default_size = 30.f;
        auto& bdz = button_default_size;
        ImVec2 pos = { static_cast<float>(win_pos.x), static_cast<float>(win_pos.y) };

        ImGui::SetNextWindowPos(pos, ImGuiCond_::ImGuiCond_Once);
        ImGui::Begin(win_name.c_str(), (bool*)false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

        vector<bool> buttons_pressed;

        for(const auto& button : buttons)
            buttons_pressed.push_back(ImGui::Button(button.c_str(), { 90, bdz }));

        ImGui::End();
    }

    void GamePlay::render_ui(const math::float3& camera_position)
    {
        vector<string> fps_info = { std::to_string(ImGui::GetIO().Framerate) + " FPS",
                                    std::to_string(1000.f / ImGui::GetIO().Framerate) + " frames/s", };

        renderText(math::int2(1220, 15), "FPS Info", fps_info);

        vector<string> camera_info = { ("X: "s + std::to_string(camera_position.x)),
                                       ("Y: "s + std::to_string(camera_position.y)),
                                       ("Z: "s + std::to_string(camera_position.z)) };

        renderText(math::int2(1366 / 2, 15), "Camera Info", camera_info);
    }

    void GamePlay::render(math::float4x4& wvp)
    {
        simple_cube->render(wvp);
    }

    void GamePlay::update(math::float4x4& wvp, const float& current)
    {
        simple_cube->update(wvp, current);
    }
}