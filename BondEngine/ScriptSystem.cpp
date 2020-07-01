// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <ScriptSystem.hpp>

#include <Error.hpp>
#include <Log.hpp>

namespace be::script
{
    ScriptSystem::ScriptSystem()
    {
        name = "Script System"s;
        lib_name = "SOL2"s;

        api_name = "Lua"s;
        api_version = string(LUA_VERSION_MAJOR)
              + "." + string(LUA_VERSION_MINOR)
              + "." + string(LUA_VERSION_RELEASE);
    }

    void ScriptSystem::initialize()
    {
        utils::log::info("{} initialization is started", name);
        utils::log::info("{} : {} {}", lib_name, api_name, api_version);

        try
        {
            auto lua_state = lua.lua_state();
            lua.open_libraries(sol::lib::base,
#ifdef _DEBUG
                               sol::lib::debug,
#endif
                               sol::lib::math,
                               sol::lib::os,
                               sol::lib::io,
                               sol::lib::string);

            auto actor_script_file_name = "game\\scripts\\actor.lua"s;
            auto mm_script_file_name = "game\\scripts\\main_menu.lua"s;
            
            auto& asfn = actor_script_file_name;
            auto ufr = lua.script_file(asfn);
            utils::log::info("Open script file {}"s, asfn);

            auto actor_name = lua.get<string>("name");
        }
        catch (sol::error& ex)
        {
            core::FatalError fe("ScriptSystem.cpp", 51u, core::Error::Type::SCRIPT, ex.what());
            fe.finalize();
        }
        
        state = State::INITIALIZED;
        utils::log::info("{} is initialized", name);
    }

    void ScriptSystem::run()
    {

    }

    void ScriptSystem::finalize()
    {
        if (state == State::INITIALIZED)
        {
            state = State::FINALIZED;
            utils::log::info("{} is finalized", name);
        }
    }
}