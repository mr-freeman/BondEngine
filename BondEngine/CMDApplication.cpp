// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <CMDApplication.hpp>

#include <spdlog/spdlog.h>
#include <Log.hpp>

namespace be::core::cmd
{
    CMDApplication::CMDApplication() :
        cmd_args(GetCommandLine()),
        nosound{ std::make_pair("-nosound"s, false) },
        nosplash{ std::make_pair("-nosplash"s, false) }
    {
        auto findEXECommand = [&](std::pair<string, bool>& command)
        {
            auto found = cmd_args.find(command.first);
            command.second = (found != std::string::npos) ? false : true;

            if (found != std::string::npos)
                return true;
            else
                return false;
        };

        if (findEXECommand(nosound))
            utils::log::info("CMD Application: Sound Device is disabled"s);

        if (findEXECommand(nosplash))
            utils::log::info("CMD Application: Splash Screen is disabled"s);

        utils::log::info("CMD Application is initialized"s);
    }
}