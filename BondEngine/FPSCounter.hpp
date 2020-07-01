// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::utils
{
    class FPSCounter
    {
    public:
        FPSCounter() = default;
       ~FPSCounter() = default;

        FPSCounter(const FPSCounter&) = delete;
        FPSCounter(FPSCounter&&) = delete;

        FPSCounter operator=(const FPSCounter&) = delete;
        FPSCounter operator=(FPSCounter&&) = delete;

    public:
        void start_calculation();
        void end_calculation(bool vsync);

        double getFPS() const { return fps; }

    private:
        clock_t current_ticks = 0,
                delta_ticks = 0,
                fps = 0;
    };
}