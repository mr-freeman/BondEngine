// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <GUISystem.hpp>
#include <Sound2D.hpp>

#include <Texture.hpp>

#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>

namespace be::game
{
    // One of three major game states.
    class MainMenu
    {
    public:
        MainMenu() :
            sound{ std::make_unique<sound::Sound2D>("mm\\main_theme") }
        {
            sound->setLoop(true);
        }

       ~MainMenu() = default;

        MainMenu(const MainMenu&) = delete;
        MainMenu(MainMenu&&) = delete;

        MainMenu operator=(const MainMenu&) = delete;
        MainMenu operator=(MainMenu&&) = delete;

    public:
        struct Branch
        {
            void initializeBuffers(int positionX, int positionY)
            {
                Diligent::Uint32 Indices[] =
                {
                    0, 1, 2, 3, 4, 5
                };

                // index_buffer->create(rend, Indices, sizeof(Indices));

                using namespace math;

                auto m_screenWidth = 1366;
                auto m_screenHeight = 768;

                auto m_bitmapWidth = 512;
                auto m_bitmapHeight = 512;

                // Calculate the screen coordinates of the left side of the bitmap.
                auto left = static_cast<float>((m_screenWidth / 2) * -1) + (float)positionX;

                // Calculate the screen coordinates of the right side of the bitmap.
                auto right = left + static_cast<decltype(left)>(m_bitmapWidth);

                // Calculate the screen coordinates of the top of the bitmap.
                auto top = (float)(m_screenHeight / 2) - (float)positionY;

                // Calculate the screen coordinates of the bottom of the bitmap.
                auto bottom = top - (float)m_bitmapHeight;
                /*
                render::utils::Vertex CubeVerts[] =
                {
                    float3(left, top, 0.0f), float2(0.0f, 0.0f),  // Top left. 
                    float3(right, bottom, 0.0f), float2(1.0f, 1.0f),  // Bottom right.
                    float3(left, bottom, 0.0f), float2(0.0f, 1.0f), // Bottom left.

                    float3(left, top, 0.0f), float2(0.0f, 0.0f), // Top left.
                    float3(right, top, 0.0f), float2(1.0f, 0.0f),  // Top right.
                    float3(right, bottom, 0.0f), float2(1.0f, 1.0f)  // Bottom right.
                };

                // vertex_buffer->create(rend, CubeVerts, sizeof(CubeVerts));
                */
            }
            void render();

            // buttons
            unique_ptr<Texture> texture;
            unique_ptr<render::utils::VertexBuffer> vertex_buffer;
            unique_ptr<render::utils::IndexBuffer> index_buffer;
        };

    public:
        void render_ui();
        const unique_ptr<sound::Sound2D>& getSound() { return sound; }

        const bool& getNewGamePressed() const { return new_game_pressed; }
        const bool& getExitPressed() const { return execute_exit; }

    private:
        unique_ptr<sound::Sound2D> sound;

        bool new_game_pressed = false;
        bool load_game_pressed = false;
        bool settings_pressed = false;
        bool exit_pressed = false;

        bool ep = false;
        bool execute_exit = false;

        float button_count = 4.f + 2.f;
        float button_size = 30.f;

        Branch main;
        Branch difficulty;
        Branch settings;
    };
}