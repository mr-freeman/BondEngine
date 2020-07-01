// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <SoundSystem.hpp>

#include <Log.hpp>

namespace be::sound
{
    SoundSystem::SoundSystem()
    {
        name = "Sound System"s;
        lib_name = "Emyl"s;

        api_name = "OpenAL"s;
        api_version = "1.1"s;
    }

    void SoundSystem::initialize()
    {
        utils::log::info("{} initialization is started", name);
        utils::log::info("{} : {} {}"s, lib_name, api_name, api_version);

        utils::log::info("Sound Device Name: {} ", Emyl::internal::Device::name);
        utils::log::info("Sound Device Extension: {} ", Emyl::internal::Device::ext);

        listener = make_unique<Emyl::Listener>();
        listener->setGlobalVolume(30.f);

        state = State::INITIALIZED;
        utils::log::info("{} is initialized", name);
    }

    void SoundSystem::run()
    {

    }

    void SoundSystem::finalize()
    {
        if (state == State::INITIALIZED)
        {
            state = State::FINALIZED;
            utils::log::info("{} is finalized", name);
        }
    }
}