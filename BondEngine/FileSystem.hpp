// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be
{
    // Here we use static everything, because it's shorter.
    class FileSystem
    {
    public:
        FileSystem() = default;
       ~FileSystem() = default;

        FileSystem(const FileSystem&) = delete;
        FileSystem(FileSystem&&) = delete;

        FileSystem operator=(const FileSystem&) = delete;
        FileSystem operator=(FileSystem&&) = delete;

    public:
        static void initialize();
        static void finalize();

    public:
        static void checkGameDirectories();
        static void checkDefaultGameFiles();

    public:
        using  path = be::fs::path;

        static path getGameDataDirectory(const string& directory_name);
        static path getUserDataDirectory(const string& directory_name);

    private:
        static path bins_data_directory;
        static path game_data_directory;
        static path user_data_directory;

        static path configs_directory;
        static path fonts_directory;
        static path levels_directory;
        static path meshes_directory;
        static path scripts_directory;
        static path shaders_directory;
        static path sounds_directory;
        static path textures_directory;

        static path logs_directory;
        static path saves_directory;
        static path screenshots_directory;
        static path settings_directory;

        static array<path, 3> engine_directories;
        static array<path, 8> game_data_directories;
        static array<path, 4> user_data_directories;

    public:
        // Classify extention by cathegory.
        // .dds / .jpg / .png are supported texture exts. etc
        // .otf / .ttf
        static string CONFIG_EXTENSION;
        static string FONT_EXTENSION;
        static string LEVEL_EXTENSION;
        static string MESH_EXTENSION;
        static string SCRIPT_EXTENSION;
        static string SHADER_EXTENSION;
        static string SOUND_EXTENSION;
        static string TEXTURE_EXTENSION;
    };
}