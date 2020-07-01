// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <ScriptSystem.hpp>

#include <File.hpp>

#include <sol/sol.hpp>

namespace be::script
{
    class Script : public File
    {
    public:
        Script(const std::string& file_name, const std::shared_ptr<ScriptSystem>& ss)
        {
            name = file_name;
            directory = FileSystem::getGameDataDirectory("sound");
            extension = FileSystem::SOUND_EXTENSION; // Elym playbacks ogg vorbis only
            full_name = this->directory / (this->name + this->extension);

            ss->getLuaState().script_file(full_name.string());

            check(true);
        }
    };
}