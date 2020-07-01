// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>
#include <FileSystem.hpp>

#include <..\third-party\LeksysINI\iniparser.hpp>

namespace be::core::cmd
{
    // Load and save core engine commands via setting files in a directory user//settings//*.ini.
    // Values could be changed with in-game CMD console.
    class CMDUserSetting
    {
    public:
        CMDUserSetting() = default;
       ~CMDUserSetting() = default;

        CMDUserSetting(const CMDUserSetting&) = delete;
        CMDUserSetting(CMDUserSetting&&) = delete;

        CMDUserSetting operator=(const CMDUserSetting&) = delete;
        CMDUserSetting operator=(CMDUserSetting&&) = delete;

    public:
        struct File
        {
            ini::File file;

            string file_name; // without extension, it is set in load function.
            fs::path file_directory = FileSystem::getUserDataDirectory("settings");
            fs::path full_file_name;
        };

        template<typename T>
        struct SettingParameter
        {
            string name = ""s; // token
            T value; // value i.e. bool, string, int, float
        };

        template<typename T>
        using Parameter = SettingParameter<T>;

        // TODO: Move Engine commands into a deque.
        struct VideoFile : public File
        {
            Parameter<string> renderer = { "renderer"s, "d3d11"s };
            Parameter<uint> width = { "width"s, 1366 };
            Parameter<uint> height = { "height"s, 768 };

            Parameter<bool> fullscreen = { "fullscreen"s, true }; // 0-1, on-off
            Parameter<bool> vsync = { "vsync"s, true };

            Parameter<bool> shadows = { "shadows"s, true };
            Parameter<uint> shadow_quality = { "shadow_quality"s, 1 };

            Parameter<bool> msaa = { "msaa"s, true };
            Parameter<uint> msaa_sample_count = { "msaa_sample_count"s, 1 };

            Parameter<bool> anisotropic_filtering = { "anisotropic_filtering"s, true };
            Parameter<uint> anisotropic_filtering_quality = { "anisotropic_filtering_quality"s, 16 };
        };

        struct SoundFile : public File
        {
            Parameter<float> ui_volume = { "ui_volume"s, 1.0 };
            Parameter<float> game_volume = { "game_volume"s, 1.0f };
        };

        struct InputFile : public File
        {
            Parameter<string> cmd_key = { "cmd_key"s, "tilda"s };;
            Parameter<string> menu_key = { "menu_key"s, "esc"s };

            Parameter<string> use_key = { "use"s, "f"s };
            Parameter<string> crouch_key = { "crouch_key"s, "ctrl"s };;
            Parameter<string> sprint_key = { "sprint_key"s, "shift"s };;
        };

    public:
        void loadAllFiles();

        VideoFile getVideoSTG() const { return video_stg; }
        SoundFile getSoundSTG() const { return sound_stg; }
        InputFile getInputSTG() const { return input_stg; }

    private:
        // might be array, by the number of engine commands changes 
        // as development of the engine goes
        //vector<Parameter<std::any>> engine_commands{ };

        VideoFile video_stg;
        SoundFile sound_stg;
        InputFile input_stg;
    };
}