// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

namespace be::script
{
    // SOL2 throws exceptions through error class derived from std::exception.
    // And so we use it to provide high-level error description.
    class ScriptSystem : public IEngine, public IEngine::Component
    {
    public:
        ScriptSystem();
       ~ScriptSystem() = default;

        ScriptSystem(const ScriptSystem&) = delete;
        ScriptSystem(ScriptSystem&&) = delete;

        ScriptSystem operator=(const ScriptSystem&) = delete;
        ScriptSystem operator=(ScriptSystem&&) = delete;

    public:
        virtual void initialize() override;
        virtual void run() override;
        virtual void finalize() override;

    public:
        sol::state& getLuaState() { return lua; }

    public:
        sol::state lua;
    };
}