// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::utils
{
    // No need to define spd_logger getter,
    // we can simply use log::info / log::warn etc
    namespace log = spdlog;

    // Bond Engine Log could be accessible from any point in source code to warn, info or error instantly.
    class Log
    {
    public:
        Log();
       ~Log() = default;

        Log(const Log&) = delete;
        Log(Log&&) = delete;

        Log operator=(const Log&) = delete;
        Log operator=(Log&&) = delete;

    private:
        shared_ptr<spdlog::logger> spd_logger;
    };
}