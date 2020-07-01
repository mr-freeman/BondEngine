// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <Log.hpp>
#include <spdlog\version.h>

#include <FileSystem.hpp>

namespace be::utils
{
    Log::Log() :
        spd_logger(spdlog::basic_logger_st("BondEngine"s,
                   FileSystem::getUserDataDirectory("logs").string() + "\\log.txt"s,
                   true))
    {
        auto logger_flag = _DEBUG ? spdlog::level::debug : spdlog::level::info;

        //spd_logger->set_level(logger_flag);
        spdlog::set_level(logger_flag);
        spdlog::set_default_logger(spd_logger);
        spdlog::set_pattern("[%Y-%d-%m %T.%f] [%l] %v."s); // %l is [info] etc, %v means the message.
        spdlog::flush_on(logger_flag); // update it in case of exception.
        
        spd_logger->info("SPD Log {}.{}.{} is initialized"s, SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    }
}