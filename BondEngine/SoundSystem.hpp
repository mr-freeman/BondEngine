// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

#include <..\third-party\Emyl\emyl.h>

namespace be::sound
{
    class SoundSystem : public IEngine, public IEngine::Component
    {
    public:
        SoundSystem();
       ~SoundSystem() = default;

        SoundSystem(const SoundSystem&) = delete;
        SoundSystem(SoundSystem&&) = delete;

        SoundSystem operator=(const SoundSystem&) = delete;
        SoundSystem operator=(SoundSystem&&) = delete;

    public:
        void initialize() override;
        void finalize() override;
        void run() override; // All the sounds play in non-main threads. So it's empty.

        const unique_ptr<Emyl::Listener>& getListener() { return listener; }

    private:
        unique_ptr<Emyl::Listener> listener;
    };
}