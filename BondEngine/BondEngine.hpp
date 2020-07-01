// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

#include <MultiSampleAA.hpp>
#include <StateSwitcher.hpp>
#include <CMDUserSetting.hpp>
#include <FPSCounter.hpp>

#include <InputHandler.hpp>
#include <ScriptSystem.hpp>
#include <SoundSystem.hpp>
#include <GUISystem.hpp>
#include <IRender.hpp>

#include <Window.hpp>

namespace be
{
    namespace info
    {
        constexpr const char* NAME = "Bond Engine";
        constexpr const char* STATUS = "Demo";

        constexpr unsigned long MAJOR_VERSION = 0;
        constexpr unsigned long MINOR_VERSION = 0;
        constexpr unsigned long PATCH_VERSION = 6;

        const string FULL_NAME = (string(info::NAME) + " " +
                          std::to_string(info::MAJOR_VERSION) + "." +
                          std::to_string(info::MINOR_VERSION) + "." +
                          std::to_string(info::PATCH_VERSION) + " " + info::STATUS);
    }

    class BondEngine : public IEngine
    {
    public:
        BondEngine() = default;
       ~BondEngine() = default;

        BondEngine(const BondEngine&) = delete;
        BondEngine(BondEngine&&) = delete;

        BondEngine operator=(const BondEngine&) = delete;
        BondEngine operator=(BondEngine&&) = delete;

    public:
        enum class Component
        {
            RENDER,
            INPUT_HANDLER,
            SOUND_SYSTEM,
            SCRIPT_SYSTEM,
            PHYSICS_SYSTEM
        };

    public:
        // i.g. Startup params
        struct Desc
        {
            math::int2 resolution;
            bool fullscreen;
            bool vsync;
            float volume;

            bool msaa;
            uint msaa_sample_count;

            bool anisotropic_filtering;
            uint anisotropic_filtering_quality;
        };

    public:
        void preinitialize() override final;

        // Initialize all the game engine components one by one.
        void initialize() override final;

        // Start game engine if initialization is successful.
        void run() override final;

        // Finalize game engine when exiting.
        void finalize() override final;

    private:
        unique_ptr<utils::MainWindow> main_window;
        unique_ptr<utils::FPSCounter> fps_counter;

        shared_ptr<gui::GUISystem> gui_system;
        unique_ptr<render::MultiSampleAA> msaa;
        unique_ptr<game::StateSwitcher> state_switcher;
        ///---------------

        // Engine Components
        shared_ptr<render::IRender> render;
        unique_ptr<script::ScriptSystem> script_system;
        unique_ptr<sound::SoundSystem> sound_system;
        unique_ptr<input::InputHandler> input_handler;
        ///---------------

        Desc startup_settings;
        vector<Command> commands;
        IEngine::State state = IEngine::State::NONE;
    };
}