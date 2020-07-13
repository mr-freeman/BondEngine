// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <any>
#include <string>

namespace be
{
    class IEngine
    {
    public:
        IEngine() = default;
        virtual ~IEngine() = default;

        IEngine(const IEngine&) = delete;
        IEngine(IEngine&&) = delete;

        IEngine operator=(const IEngine&) = delete;
        IEngine operator=(IEngine&&) = delete;

    public:
        virtual void initialize() = 0;
        virtual void run() = 0;
        virtual void finalize() = 0;

    public:
        enum class State
        {
            NONE,
            INITIALIZED,
            FINALIZED
        };

        struct Component
        {
            string name;
            string lib_name;

            string api_name;
            string api_version;

            State state = State::NONE;
        };

        struct Command
        {
            string name = ""s; // token
            std::any value; // value i.e. bool, string, int, float
        };
    };
}