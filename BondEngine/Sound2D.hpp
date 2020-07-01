// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <File.hpp>

#include <Sound.hpp>

#include <filesystem>

namespace be::sound
{
    class Sound2D : public Emyl::Music, public Sound, public File
    {
    public:
        Sound2D(const std::string& file_name)
        {
            name = file_name;
            directory = FileSystem::getGameDataDirectory("sound");
            extension = FileSystem::SOUND_EXTENSION; // Elym playbacks ogg vorbis only
            full_name = this->directory / (this->name + this->extension);

            check(true);

            this->openFromFile(full_name.string());
        }

        virtual ~Sound2D() = default;

        Sound2D(const Sound2D&) = default;
        Sound2D(Sound2D&&) = delete;

        Sound2D operator=(const Sound2D&) = delete;
        Sound2D operator=(Sound2D&&) = delete;
    };

    using SoundUI = Sound2D;
    using Ambient = Sound2D;
}