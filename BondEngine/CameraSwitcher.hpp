// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <FPSCamera.hpp>

namespace be::game
{
    class CameraSwitcher
    {
         explicit CameraSwitcher(const FPSCamera& fps_camera);
                 ~CameraSwitcher();

    public:
        void setMain();

    private:
        std::vector<FPSCamera> cameras;
    };
}