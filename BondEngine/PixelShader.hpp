// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IShader.hpp>

namespace be::render::utils
{
    class PixelShader : public IShader
    {
    public:
        PixelShader() = default;
        ~PixelShader() = default;

        PixelShader(const PixelShader&) = delete;
        PixelShader(PixelShader&&) = delete;

        PixelShader operator=(const PixelShader&) = delete;
        PixelShader operator=(PixelShader&&) = delete;

    public:
        Diligent::IShader* get() const override final { return shader; }

        void open(const std::string& file_name) override final;
        void create(const shared_ptr<render::IRender>& rend, ShaderCreateInfo& sci) override final;
    };
}