// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IndexBuffer.hpp>
#include <VertexBuffer.hpp>

#include <PixelShader.hpp>
#include <VertexShader.hpp>

namespace be::game
{
    class SkyBox
    {
    public:
        SkyBox() = default;
        ~SkyBox() = default;

        SkyBox(const SkyBox&) = default;
        SkyBox(SkyBox&&) = default;

        SkyBox operator=(const SkyBox&) = delete;
        SkyBox operator=(SkyBox&&) = delete;

    public:
        void initialize();
        // render

    private:
        shared_ptr<render::utils::IndexBuffer> index_buffer;
        shared_ptr<render::utils::VertexBuffer> vertex_buffer;

        // texture

        shared_ptr<render::utils::VertexShader> vertex_shader;
        shared_ptr<render::utils::PixelShader> pixel_shader;
    };
}