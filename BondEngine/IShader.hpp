// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>

namespace be::render::utils
{
    class IShader
    {
    public:
        IShader() = default;
        virtual ~IShader() = default;

        IShader(const IShader&) = delete;
        IShader(IShader&&) = delete;

        IShader operator=(const IShader&) = delete;
        IShader operator=(IShader&&) = delete;

    public:
        virtual Diligent::IShader* get() const = 0;
        virtual void open(const std::string& file_name) = 0;
        virtual void create(const shared_ptr<render::IRender>& rend, Diligent::ShaderCreateInfo& sci) = 0;

    protected:
        Diligent::IShader* shader;
        Diligent::SHADER_TYPE type;

        string desc_name;
        string full_name;
    };
}