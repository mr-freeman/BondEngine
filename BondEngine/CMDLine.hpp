// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

namespace be::core::cmd
{
    class CMDLine
    {
    public:
        CMDLine() = default;
       ~CMDLine() = default;

    public:
        void initialize(vector<IEngine::Command>& commands);
        void finalize();
    };
}