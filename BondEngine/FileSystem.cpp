// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <FileSystem.hpp>

#include <Error.hpp>
#include <Log.hpp>

namespace be
{
    using path = be::fs::path;

    string FileSystem::CONFIG_EXTENSION = ".cfg"s;
    string FileSystem::FONT_EXTENSION = ".ttf"s;
    string FileSystem::LEVEL_EXTENSION = ".lvl"s;
    string FileSystem::MESH_EXTENSION = ".dae"s;
    string FileSystem::SCRIPT_EXTENSION = ".lua"s;
    string FileSystem::SHADER_EXTENSION = ".hlsl"s;
    string FileSystem::SOUND_EXTENSION = ".ogg"s;
    string FileSystem::TEXTURE_EXTENSION = ".dds"s;

    path FileSystem::bins_data_directory = fs::current_path() / "bins";
    path FileSystem::game_data_directory = fs::current_path() / "game";
    path FileSystem::user_data_directory = fs::current_path() / "user";

    array<path, 3> FileSystem::engine_directories{ FileSystem::bins_data_directory,
                                                   FileSystem::game_data_directory, 
                                                   FileSystem::user_data_directory };

    path FileSystem::configs_directory = (game_data_directory / "configs"s);
    path FileSystem::fonts_directory = (game_data_directory / "fonts"s);
    path FileSystem::levels_directory = (game_data_directory / "levels"s);
    path FileSystem::meshes_directory = (game_data_directory / "meshes"s);
    path FileSystem::scripts_directory = (game_data_directory / "scripts"s);
    path FileSystem::shaders_directory = (game_data_directory / "shaders"s);
    path FileSystem::sounds_directory = (game_data_directory / "sounds"s);
    path FileSystem::textures_directory = (game_data_directory / "textures"s);

    array<path, 8> FileSystem::game_data_directories{ configs_directory,
                                                      fonts_directory,
                                                      levels_directory,
                                                      meshes_directory,
                                                      scripts_directory,
                                                      shaders_directory,
                                                      sounds_directory,
                                                      textures_directory };

    path FileSystem::logs_directory = (user_data_directory / "logs"s);
    path FileSystem::saves_directory = (user_data_directory / "saves"s);
    path FileSystem::screenshots_directory = (user_data_directory / "screenshots"s);
    path FileSystem::settings_directory = (user_data_directory / "settings"s);

    array<path, 4> FileSystem::user_data_directories{ logs_directory,
                                                      saves_directory,
                                                      screenshots_directory,
                                                      settings_directory };

    void FileSystem::initialize()
    {
        checkGameDirectories();
        checkDefaultGameFiles();

        utils::log::info("File System is initialized"s);
    }

    void FileSystem::finalize()
    {
        utils::log::info("File System is finalized"s);
    }

    void FileSystem::checkGameDirectories()
    {
        auto checkGameDirectory = [](const fs::path& directory)
        {
            if (!fs::exists(directory))
            {
                auto desc = "Can't find directory " + directory.string();

                core::FatalError fe("FileSystem.cpp", 80u, core::Error::Type::FILESYSTEM, desc.c_str());
                fe.finalize();
            }
        };

        for (auto engine_directory : engine_directories)
            checkGameDirectory(engine_directory);

        for (auto& game_directory : game_data_directories)
            checkGameDirectory(game_directory);
        
        for (auto& user_directory : user_data_directories)
            if (!fs::exists(user_directory))
            {
                auto user_dir = user_directory.string().c_str();
                utils::log::warn("Can't find directory {}"s, user_dir);
                fs::create_directories(user_directory);
                utils::log::info("Directory {} is created"s, user_dir);
            }
    }

    void FileSystem::checkDefaultGameFiles()
    {
        auto checkDefaultGameFile = [](const string& ext, const string& data_dir)
        {
            if (!fs::exists(getGameDataDirectory(data_dir) / ("default" + ext)))
            {
                auto desc = "Can't open default " + data_dir + " file";

                core::FatalError fe("FileSystem.cpp", 109u, core::Error::Type::FILESYSTEM, desc.c_str());
                fe.finalize();
            }
        };

        auto file = "font"s;
        checkDefaultGameFile(FONT_EXTENSION, file);

        file = "sound"s;
        checkDefaultGameFile(SOUND_EXTENSION, file);

        file = "texture"s;
        checkDefaultGameFile(TEXTURE_EXTENSION, file);
    }

    path FileSystem::getGameDataDirectory(const string& directory)
    {
        for (auto& game_d_directory : game_data_directories)
        {
            if ((game_d_directory.string().find(directory)) != std::string::npos)
                return game_d_directory;
        }

        return {};
    }

    path FileSystem::getUserDataDirectory(const string& directory)
    {
        for (auto& user_d_directory : user_data_directories)
        {
            if ((user_d_directory.string().find(directory)) != std::string::npos)
                return user_d_directory;
        }

        return {};
    }
}