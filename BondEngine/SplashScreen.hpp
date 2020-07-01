// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <Window.hpp>

namespace be::utils
{
    class SplashScreen : virtual public Window
    {
    public:
        SplashScreen();
        virtual ~SplashScreen() = default;

        SplashScreen(const SplashScreen&) = delete;
        SplashScreen(SplashScreen&&) = delete;

        SplashScreen operator=(const SplashScreen&) = delete;
        SplashScreen operator=(SplashScreen&&) = delete;

    public:
        void create() override;
        void registerClass() override;
        bool update();

        void show();
        void hide() const;

        void loadBitMap();
        void isShown(bool is_shown) const override;
        HWND getHandle() const override { return winhandle; }

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HBITMAP bitmap;
        BITMAPINFO bitmapInfo;

        HINSTANCE m_instance;
    };
}