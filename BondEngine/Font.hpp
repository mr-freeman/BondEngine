// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <File.hpp>

#include <..\DiligentTools\ThirdParty\imgui\imgui.h>

namespace be
{
    class Font
    {
    public:
        Font() :
            font(nullptr),
            size(16.f)
        {}

        virtual ~Font()
        {
            if (font)
                font = nullptr;
        };

    public:
        void load(const std::filesystem::path& file_name)
        {
            auto& io = ImGui::GetIO();

            config->RasterizerMultiply = 1.0f;
            font = io.Fonts->AddFontFromFileTTF(file_name.string().c_str(), size, config);
            io.Fonts->Build();
        }

        void setSize(const float& size)
        {
            this->size = size;
        }

        void push()
        {
            ImGui::PushFont(font);
        }

        void pop()
        {
            ImGui::PopFont();
        }

    private:
        ImFont* font;
        ImFontConfig* config;

        float size;
    };

    class UnifiedFontFile : public File, public Font
    {
    public:
        UnifiedFontFile(const std::string& file_name, const std::filesystem::path& file_extension)
        {
            name = file_name;
            directory = FileSystem::getGameDataDirectory("font");
            extension = file_extension.string();
            full_name = this->directory / (this->name + this->extension);

            check(true);
        }

       ~UnifiedFontFile() = default;
    };
}