// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <Window.hpp>
#include <Resource.h>

#include <Timer.hpp>

#include <..\DiligentTools\ThirdParty\imgui\imgui.h>

namespace be::utils
{
    MainWindow::MainWindow(const string& name, const math::int2& resolution, const bool& fullscreen) :
        width(resolution.x),
        height(resolution.y),
        fullscreen(fullscreen)
    { 
        winclass = { 0 };
        winhandle = { 0 };

        this->name = name;
        this->class_name = "BE Window Class"s;

        SetUpCallBacks();
    }

    void MainWindow::registerClass()
    {
        winclass.cbSize = sizeof(WNDCLASSEX);
        winclass.style = CS_HREDRAW | CS_VREDRAW;
        winclass.lpfnWndProc = WNDPROC(_WndProc);
        winclass.cbClsExtra = 0;
        winclass.cbWndExtra = 0;
        winclass.hInstance = GetModuleHandleA(nullptr);
        winclass.hIcon = LoadIconA(nullptr, MAKEINTRESOURCE(IDI_BONDENGINE));
        winclass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
        winclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        winclass.lpszMenuName = nullptr;
        winclass.lpszClassName = class_name.c_str();
        winclass.hIconSm = LoadIconA(nullptr, MAKEINTRESOURCE(IDI_SMALL));

        if (!RegisterClassExA(&winclass))
            MessageBoxA(nullptr, "Can't register window class", "Error", MB_OK | MB_ICONERROR);
    }

    void MainWindow::create()
    {
        DWORD style, style_ex;

        if (fullscreen)
        {
            style = WS_POPUP;
            style_ex = WS_EX_APPWINDOW;
        }
        else
        {
            if (is_resizable)
                style = WS_OVERLAPPEDWINDOW;
            else
                style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;

            style_ex = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        }

        style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;


        hwnd = CreateWindowExA(style_ex,
                               class_name.c_str(),
                               name.c_str(),
                               style,
                               0,
                               0,
                               width,
                               height,
                               nullptr,
                               nullptr,
                               GetModuleHandleA(name.c_str()),
                               this);

        if (!hwnd)
            MessageBoxA(nullptr, "Can't create window", "Error", MB_OK | MB_ICONERROR);

        // Center the window
        RECT rect;
        GetWindowRect(hwnd, &rect);
        const int screenX = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
        const int screenY = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;

        SetWindowPos(hwnd, hwnd, screenX, screenY, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }

    bool MainWindow::update(gainput::InputManager& mgr, const shared_ptr<gui::GUISystem>& gui_system)
    {
        if (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);

            gui_system->getImGUI()->Win32_ProcHandler(hwnd, msg.message, msg.wParam, msg.lParam);

            mgr.HandleMessage(msg);
        }
        else
        {
            CurrTime = Timer.GetElapsedTime();
            ElapsedTime = CurrTime - PrevTime;
            PrevTime = CurrTime;
        }

        if (msg.message == WM_QUIT)
            return false;

        return true;
    }

    void MainWindow::isShown(bool is_shown) const
    {
        ShowWindow(hwnd, is_shown ? SW_SHOW : SW_HIDE);
        UpdateWindow(hwnd);
        InvalidateRect(hwnd, nullptr, FALSE);
    }

    void MainWindow::isCursorShown(bool is_shown) const
    {
        ShowCursor(is_shown ? TRUE : FALSE);
    }
}