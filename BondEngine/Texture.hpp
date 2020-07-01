// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>
#include <File.hpp>

namespace be
{
    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;

        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;

        Texture operator=(const Texture&) = delete;
        Texture operator=(Texture&&) = delete;

    public:
        void load(const std::filesystem::path& file_name, Diligent::IRenderDevice* device)
        {
            Diligent::TextureLoadInfo loadInfo;
            loadInfo.IsSRGB = Diligent::False;

            CreateTextureFromFile(file_name.string().c_str(), loadInfo, device, &texture);

            texture_view = texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        }

        Diligent::ITextureView* getTextureView() const { return texture_view; }

    public:
        Diligent::ITexture* texture = nullptr;
        Diligent::ITextureView* texture_view = nullptr;
    };

    // Diligent Graphics Engine Utils allow to load .dds/.jpg/.png via one single function.
    class UnifiedTextureFile : public Texture, public File
    {
    public:
        UnifiedTextureFile(const std::string& file_name, const std::filesystem::path& file_extension)
        { 
            name = file_name;
            directory = FileSystem::getGameDataDirectory("texture");
            extension = file_extension.string();
            full_name = this->directory / (this->name + this->extension);

            check(true);
        }
        
       virtual ~UnifiedTextureFile() = default;

    public:
        std::filesystem::path getFullName() const { return full_name; }
    };
}