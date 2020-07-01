// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <PixelShader.hpp>

#include <FileSystem.hpp>
#include <Log.hpp>

namespace be::render::utils
{
    void PixelShader::open(const std::string& file_name)
    {
        type = SHADER_TYPE_PIXEL;
        shader = nullptr;
        desc_name = file_name;
        full_name = (FileSystem::getGameDataDirectory("shaders"s) / (file_name + FileSystem::SHADER_EXTENSION)).string();

        if (!fs::exists(full_name))
        {
            be::utils::log::info("Can't open shader file {}", full_name);
            exit(2);
        }

        be::utils::log::info("Open shader file {}", full_name);
    }

    void PixelShader::create(const shared_ptr<render::IRender>& rend, Diligent::ShaderCreateInfo& sci)
    {
        sci.Desc.ShaderType = type;
        sci.EntryPoint = "main";
        sci.Desc.Name = desc_name.c_str();
        sci.FilePath = full_name.c_str();

        rend->getRenderDevice()->CreateShader(sci, &shader);
    }
}