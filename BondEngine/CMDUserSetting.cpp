// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <CMDUserSetting.hpp>

#include <Log.hpp>

namespace be::core::cmd
{
    void CMDUserSetting::loadAllFiles()
    {
        auto loadFile = [&](File& file, const string& name) -> bool
        {
            file.file_name = name + ".ini"s;
            file.full_file_name = file.file_directory / fs::path(file.file_name);

            if (!file.file.Load(file.full_file_name.string()))
                return false;

            return true;
        };

        auto createDefaultFile = [&](File& default_file)
        {
            auto writeDefault = [&](const string& key, const ini::Value& value)
            {
                default_file.file.GetSection("main")->SetValue(key, value);
            };

            if (default_file.file_name == "video.ini"s)
            {
                writeDefault(video_stg.renderer.name, video_stg.renderer.value);
                writeDefault(video_stg.width.name, video_stg.width.value);
                writeDefault(video_stg.height.name, video_stg.height.value);

                writeDefault(video_stg.fullscreen.name, video_stg.fullscreen.value);
                writeDefault(video_stg.vsync.name, video_stg.vsync.value);
                writeDefault(video_stg.shadows.name, video_stg.shadows.value);

                default_file.file.Save(default_file.full_file_name.string());
            }
            else if (default_file.file_name == "sound.ini"s)
            {
                writeDefault(sound_stg.game_volume.name, "1.0");

                default_file.file.Save(default_file.full_file_name.string());
            }
            else if (default_file.file_name == "input.ini")
            {

            }
            else
                utils::log::warn("Unknown setting file {}.", default_file.file_name);
        };

        if (!loadFile(video_stg, "video"s))
            createDefaultFile(video_stg);
        else
        {
            video_stg.renderer.value = video_stg.file.GetSection("main")->GetValue(video_stg.renderer.name, video_stg.renderer.value).AsString();
            if (video_stg.renderer.value != "d3d11"s)
            {
                utils::log::warn("The engine currently supports Direct3D 11 only.");
                video_stg.renderer.value = "d3d11"s;
            }

            video_stg.fullscreen.value = video_stg.file.GetSection("main")->GetValue(video_stg.fullscreen.name,
                                                                                     video_stg.fullscreen.value).AsBool();
            video_stg.vsync.value = video_stg.file.GetSection("main")->GetValue(video_stg.vsync.name,
                                                                                video_stg.vsync.value).AsBool();

            video_stg.width.value = video_stg.file.GetSection("main")->GetValue(video_stg.width.name).AsInt();
            video_stg.height.value = video_stg.file.GetSection("main")->GetValue(video_stg.height.name).AsInt();

            // Shadows
            {
                video_stg.shadows.value = video_stg.file.GetSection("main")->GetValue(video_stg.shadows.name, video_stg.shadows.value).AsBool();
                video_stg.shadow_quality.value = video_stg.file.GetSection("main")->GetValue(video_stg.shadow_quality.name,
                                                                                             video_stg.shadow_quality.value).AsInt();

                if (video_stg.shadow_quality.value > 4)
                    video_stg.shadow_quality.value = 4;

                if (video_stg.shadow_quality.value < 1)
                    video_stg.shadow_quality.value = 1;
            }

            // MSAA
            {
                video_stg.msaa.value = video_stg.file.GetSection("main")->GetValue(video_stg.msaa.name, video_stg.msaa.value).AsBool();
                video_stg.msaa_sample_count.value = video_stg.file.GetSection("main")->GetValue(video_stg.msaa_sample_count.name,
                                                                                                video_stg.msaa_sample_count.value).AsInt();

                if (!(video_stg.msaa_sample_count.value % 2 == 0))
                    video_stg.msaa_sample_count.value = 1;

                if (video_stg.msaa_sample_count.value == 6)
                    video_stg.msaa_sample_count.value = 4;

                if (video_stg.msaa_sample_count.value > 8)
                    video_stg.msaa_sample_count.value = 8;

                if (video_stg.msaa_sample_count.value < 1)
                    video_stg.msaa_sample_count.value = 1;
            }

            // Anisotropic filtering
            {
                video_stg.anisotropic_filtering.value = video_stg.file.GetSection("main")->GetValue(video_stg.anisotropic_filtering.name,
                                                                                                    video_stg.anisotropic_filtering.value).AsBool();

                video_stg.anisotropic_filtering_quality.value = video_stg.file.GetSection("main")->GetValue(
                    video_stg.anisotropic_filtering_quality.name,
                    video_stg.anisotropic_filtering_quality.value).AsInt();

                if (!(video_stg.anisotropic_filtering_quality.value % 2 == 0))
                    video_stg.anisotropic_filtering_quality.value = 1;

                if (video_stg.anisotropic_filtering_quality.value > 16)
                    video_stg.anisotropic_filtering_quality.value = 16;

                if (video_stg.anisotropic_filtering_quality.value < 1)
                    video_stg.anisotropic_filtering_quality.value = 1;
            }

            video_stg.file.Unload();
        }

        if (!loadFile(sound_stg, "sound"s))
            createDefaultFile(sound_stg);
        else
        {
            sound_stg.game_volume.value = sound_stg.file.GetSection("main")->GetValue(sound_stg.game_volume.name,
                                                                                      sound_stg.game_volume.value).AsT<float>();
        }

        if (!loadFile(input_stg, "input"s))
            createDefaultFile(input_stg);
    }
}