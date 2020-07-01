// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IShader.hpp>

namespace be::render::utils
{
    class GeometryShader : public IShader
    {
    public:
        GeometryShader() = default;
       ~GeometryShader() = default;

        GeometryShader(const GeometryShader&) = delete;
        GeometryShader(GeometryShader&&) = delete;

        GeometryShader operator=(const GeometryShader&) = delete;
        GeometryShader operator=(GeometryShader&&) = delete;

    public:
        Diligent::IShader* get() const override final { return shader; }

        void open(const std::string& file_name) override final;
        void create(const shared_ptr<render::IRender>& rend, ShaderCreateInfo& sci) override final;
    };
}