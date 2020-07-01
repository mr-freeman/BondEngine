// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>

#include <ILight.hpp>

#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>

#include <GeometryShader.hpp>
#include <VertexShader.hpp>
#include <PixelShader.hpp>

#include <Texture.hpp>

namespace be::game
{
    struct Constants
    {
        math::float4x4 WorldViewProj;
        math::float4x4 World;
    };

    struct ConstantsLight
    {
        utils::render::DirectionalLight light;
    };

    class SimpleCube
    {
    public:
        SimpleCube();
       ~SimpleCube();

        SimpleCube(const SimpleCube&) = delete;
        SimpleCube(SimpleCube&&) = delete;

        SimpleCube operator=(const SimpleCube&) = delete;
        SimpleCube operator=(SimpleCube&&) = delete;

    public:
        void setRenderer(shared_ptr<render::IRender>& render) { rend = render; }
        void loadTexture();

        void initialize();
        void update(math::float4x4& wvp, const float& current);
        void render(math::float4x4& wvp);

    public:
        shared_ptr<render::IRender> rend;

        unique_ptr<render::utils::GeometryShader> geometry_shader;
        unique_ptr<render::utils::VertexShader> vertex_shader;
        unique_ptr<render::utils::PixelShader> pixel_shader;

        unique_ptr<render::utils::VertexBuffer> vertex_buffer;
        unique_ptr<render::utils::IndexBuffer> index_buffer;

        unique_ptr<UnifiedTextureFile> texture;

        Diligent::IBuffer* constant_buffer = nullptr;
        Diligent::IBuffer* constant_buffer_light = nullptr;

        Diligent::IShaderResourceBinding* shader_res_bind = nullptr;

        utils::render::DirectionalLight light;
    };
}