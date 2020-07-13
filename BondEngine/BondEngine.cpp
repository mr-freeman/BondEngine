// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <BondEngine.hpp>

#include <D3D11Render.hpp>
#include <SplashScreen.hpp>
#include <FileSystem.hpp>
#include <SystemInfo.hpp>

#include <CMDApplication.hpp>
#include <CMDUserSetting.hpp>

#include <Build.hpp>
#include <Timer.hpp>
#include <Log.hpp>

namespace be
{
    void BondEngine::initialize()
    {
        // Pre initialization start (engine params)
        auto start_timer = std::chrono::system_clock::now();

        utils::Log();

        auto splash_screen = make_unique<utils::SplashScreen>();
        auto cmd_application = make_unique<core::cmd::CMDApplication>();

        if (cmd_application->isNoSplash())
        {
            splash_screen->registerClass();
            splash_screen->create();

            splash_screen->isShown(true);
            splash_screen->show();
            splash_screen->update();
        }

        auto build = make_unique<utils::Build>();
        const auto BUILD_ID = build->calculate_build_id(__DATE__);

        utils::log::info("{}, Build ID {}", info::FULL_NAME, BUILD_ID);
        utils::log::info("Initialization is started"s);

        auto system_info = make_unique<utils::SystemInfo>();
        system_info->writeToLog();

        FileSystem::initialize();

        auto user_settings = make_unique<core::cmd::CMDUserSetting>();
        user_settings->loadAllFiles();

        startup_settings.fullscreen = user_settings->getVideoSTG().fullscreen.value;
        startup_settings.resolution.x = user_settings->getVideoSTG().width.value;
        startup_settings.resolution.y = user_settings->getVideoSTG().height.value;

        startup_settings.vsync = user_settings->getVideoSTG().vsync.value;
        startup_settings.msaa = user_settings->getVideoSTG().msaa.value;
        startup_settings.msaa_sample_count = user_settings->getVideoSTG().msaa_sample_count.value;

        startup_settings.volume = user_settings->getSoundSTG().game_volume.value;

        cmd_line = make_unique<core::cmd::CMDLine>();
        cmd_line->initialize(commands);
        // Pre initialization end

        main_window = make_unique<be::utils::MainWindow>(info::FULL_NAME,
                                                         startup_settings.resolution,
                                                         startup_settings.fullscreen);

        main_window->registerClass();
        main_window->create();

        sound_system = make_unique<sound::SoundSystem>();

        if (cmd_application->isNoSound())
            sound_system->initialize();

        script_system = make_unique<script::ScriptSystem>();
        script_system->initialize();

        input_handler = make_unique<input::InputHandler>();
        input_handler->preinitialize(startup_settings.resolution);
        input_handler->initialize();

        render = make_shared<render::D3D11Render>();
        render->preinitialize(main_window->getHandle(), 
                              startup_settings.resolution,
                              startup_settings.fullscreen);
        render->initialize();

        if (startup_settings.msaa)
        {
            msaa = make_unique<render::MultiSampleAA>();

            msaa->setRenderer(render);
            msaa->determine();
            msaa->createRenderTarget(startup_settings.msaa_sample_count);
        }

        gui_system = make_shared<gui::GUISystem>();
        gui_system->preinitialize(startup_settings.resolution, 
                                  main_window->getHandle(),
                                  render->getRenderDevice());
        gui_system->initialize();

        splash_screen->hide();

        auto end_timer = std::chrono::system_clock::now();
        auto init_time = static_cast<double>(std::chrono::duration_cast
                                             <std::chrono::milliseconds>
                                             (end_timer - start_timer).count());

        if (render->state != IEngine::State::INITIALIZED ||
            script_system->state != IEngine::State::INITIALIZED ||
            input_handler->state != IEngine::State::INITIALIZED)
            state = State::NONE;
        else
            state = State::INITIALIZED;

        if (state == State::INITIALIZED)
            utils::log::info("{} was initialized in {} ms"s, info::NAME, init_time);
        else
            utils::log::info("{} was not initialized"s, info::NAME);
    }

    void BondEngine::run()
    {
        // Post initialization start (game resources)
        if (state != State::INITIALIZED)
            return;

        auto start_timer = std::chrono::system_clock::now();

        utils::log::info("Game Data initialization"s);

        gui_system->loadDefaultFont();

        state_switcher = make_unique<game::StateSwitcher>();
        state_switcher->setRenderer(render);
        
        main_window->isShown(true);

        fps_counter = make_unique<utils::FPSCounter>();

        auto end_timer = std::chrono::system_clock::now();
        auto init_time = static_cast<double>(std::chrono::duration_cast
                                             <std::chrono::milliseconds>
                                             (end_timer - start_timer).count());

        utils::log::info("Game Data was initialized in {} ms"s, info::NAME, init_time);

        if (startup_settings.fullscreen)
            render->setFullScreen(startup_settings.resolution);
        else
            render->setWindowed();

        // Dear imgui windows that don't have initial size are not rendered in the first frame,
        // see https://github.com/ocornut/imgui/issues/2949
        render->render(msaa->getRTV(), msaa->getDSV());
        render->present(1);

        // Post initialization end
        //------------------------------------------------------
        // Main Loop
        for (;;)
        {
            auto doUpdate = main_window->update(input_handler->getManager(), gui_system);
            if (!doUpdate)
                break;

            fps_counter->start_calculation();

            render->render(msaa->getRTV(), msaa->getDSV());

            doUpdate = state_switcher->update(main_window->getCurrentTime(), main_window->getElapsedTime(), input_handler->getMap());
            if (!doUpdate)
                break;

            input_handler->run();

            // GUI Loop
            gui_system->newFrame();

            state_switcher->render_ui(fps_counter->getFPS());

            gui_system->render(render->getDeviceContext());
            gui_system->endFrame();
            //--------------------------------

            if (startup_settings.msaa)
                msaa->render();

            render->present(startup_settings.vsync);
            fps_counter->end_calculation(startup_settings.vsync);
        }
    }

    void BondEngine::finalize()
    {
        if (state != State::INITIALIZED)
            return;

        // Sound System can be disabled manually via app args.
        if (sound_system->state == State::INITIALIZED)
            sound_system->finalize();

        FileSystem::finalize();

        state = State::FINALIZED;
        utils::log::info("{} is finalized"s, info::NAME);
    }
}