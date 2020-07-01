// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IShader.hpp>

namespace be::render::utils
{
    class VertexShader : public IShader
    {
    public:
        VertexShader() = default;
       ~VertexShader() = default;

        VertexShader(const VertexShader&) = delete;
        VertexShader(VertexShader&&) = delete;

        VertexShader operator=(const VertexShader&) = delete;
        VertexShader operator=(VertexShader&&) = delete;

    public:
        Diligent::IShader* get() const override final { return shader; }

        void open(const std::string& file_name) override final;
        void create(const shared_ptr<render::IRender>& rend, Diligent::ShaderCreateInfo& sci) override final;
    };
}