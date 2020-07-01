// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#undef NOWINOFFSETS

#include <WinUser.h>
#include <Windows.h>

#include <GUISystem.hpp>
#include <Timer.hpp>

namespace be::utils
{
    // Avoid Singletons in order to access static WinProc function.
    // Just inherit and set up messages this way: 
    // AddMessage(WM_CREATE, this, &MyWindowFoo::onCreate);
    // AddMessage(WM_CLOSE, this, &MyWindowFoo::onClose);
    class WindowMessageProcessor
    {
        using WndProcFunc = LRESULT(WindowMessageProcessor::*)(LPARAM, WPARAM);

        struct POINTER
        {
            WindowMessageProcessor* wnd; //Указатель на класс, которому принадлежит обработчик
            WndProcFunc func;
        };

    public:
                 WindowMessageProcessor() = default;
        virtual ~WindowMessageProcessor() = default;

    public:
        static LRESULT CALLBACK _WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
        {
            WindowMessageProcessor* wnd = nullptr;

#pragma warning(push)
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
            // Сообщения WM_NCCREATE приходит до WM_CREATE
            // т.е при создании окна
            if (message == WM_NCCREATE)
            {
                // Получаем указатель на экземпляр нашего окна, который мы передали в функцию CreateWindowEx
                wnd = reinterpret_cast<WindowMessageProcessor*>(LPCREATESTRUCT(lparam)->lpCreateParams);
                //И сохраняем в поле GWL_USERDATA
                SetWindowLongA(hwnd, GWLP_USERDATA, LONG(LPCREATESTRUCT(lparam)->lpCreateParams));
                wnd->hwnd = hwnd;
            }

            //Теперь получаем указатель на наш экземлпяр окна, но уже из поля GWL_USERDATA
            wnd = reinterpret_cast<WindowMessageProcessor*>(GetWindowLongA(hwnd, GWLP_USERDATA));
#pragma warning(pop)

            if (wnd)
            {
                std::map<UINT, POINTER>::iterator it;
                it = wnd->_msgmap.find(message);

                //Если сообщение не найдено, то обрабатываем его по умолчанию
                if (it == wnd->_msgmap.end()) 
                    return DefWindowProcA(hwnd, message, wparam, lparam);
                else
                {
                    auto msg = it->second;
                    LRESULT result = (msg.wnd->*msg.func)(lparam, wparam); //Вызываем функцию обработчик

                    if (result) 
                        return result;
                }
            }

            return DefWindowProc(hwnd, message, wparam, lparam);
        }

        template<typename T>
        bool AddMessage(UINT message, WindowMessageProcessor* wnd, LRESULT(T::*funcpointer)(LPARAM, WPARAM))
        {
            if (!wnd || !funcpointer) 
                return false;

            POINTER msg;
            msg.wnd = wnd;
            msg.func = reinterpret_cast<WndProcFunc>(funcpointer);

            _msgmap.insert(std::pair<UINT, POINTER>(message, msg));

            return true;
        }

    protected:
        HWND hwnd = { 0 };
        std::map<UINT, POINTER> _msgmap;
    };

    class Window
    {
    public:
                 Window() = default;
        virtual ~Window() = default;

        Window(const Window&) = delete;
        Window(Window&&) = delete;

        Window operator=(const Window&) = delete;
        Window operator=(Window&&) = delete;

    public:
        virtual void registerClass() = 0;
        virtual void create() = 0;

        virtual void isShown(bool is_shown) const = 0;
        virtual HWND getHandle() const = 0;

    protected:
        WNDCLASSEX winclass;
        HWND winhandle;

        string name, class_name;
    };

    class MainWindow : public WindowMessageProcessor
    {
    public:
        MainWindow(const string& name, const math::int2& resolution, const bool& fullscreen);

        MainWindow() = default;
        virtual ~MainWindow() = default;

        MainWindow(const MainWindow&) = delete;
        MainWindow(MainWindow&&) = delete;

        MainWindow operator=(const MainWindow&) = delete;
        MainWindow operator=(MainWindow&&) = delete;

    public:
        virtual void registerClass();
        virtual void create();
        
        bool update(gainput::InputManager& mgr, const shared_ptr<gui::GUISystem>& gui_system);

        virtual void isShown(bool is_shown) const;
        virtual void isCursorShown(bool is_shown) const;

        virtual HWND getHandle() const { return hwnd; }
        virtual double getElapsedTime() const { return PrevTime; }
        virtual double getCurrentTime() const { return CurrTime; }
        virtual POINT getCursorPos() { return MousePosition; }

    private:
        void SetUpCallBacks()
        {
            AddMessage(WM_SIZE, this, &MainWindow::onSize);
            AddMessage(WM_PAINT, this, &MainWindow::onPaint);

            AddMessage(WM_CLOSE, this, &MainWindow::onClose);
            AddMessage(WM_CREATE, this, &MainWindow::onCreate);
            AddMessage(WM_DESTROY, this, &MainWindow::onDestroy);

            AddMessage(WM_EXITSIZEMOVE, this, &MainWindow::onExitSizeMove);
            AddMessage(WM_ENTERSIZEMOVE, this, &MainWindow::onEnterSizeMove);

            AddMessage(WM_KILLFOCUS, this, &MainWindow::onKillFocus);
            AddMessage(WM_SETFOCUS, this, &MainWindow::onSetFocus);

            AddMessage(WM_GETMINMAXINFO, this, &MainWindow::onGetMinMaxInfo);
        }

        LRESULT onGetMinMaxInfo(LPARAM lparam, WPARAM wparam)
        {
            LPMINMAXINFO lpMMI = reinterpret_cast<LPMINMAXINFO>(lparam);
            lpMMI->ptMinTrackSize.x = 640;
            lpMMI->ptMinTrackSize.y = 480;

            return 0;
        }

        LRESULT onSetFocus(LPARAM lparam, WPARAM wparam)
        {
            //MessageBoxA(nullptr, "1", "SetFocus", 0);

            return 0;
        }

        LRESULT onKillFocus(LPARAM lparam, WPARAM wparam)
        {
            //MessageBoxA(nullptr, "1", "KillFocus", 0);

            return 0;
        }

        LRESULT onEnterSizeMove(LPARAM lparam, WPARAM wparam)
        {
            MessageBoxA(nullptr, "1", "onEnterSizeMove", 0);
            //Sleep(100);
            // We want to avoid trying to resizing the swapchain as the user does the 'rubber band' resize
            s_in_sizemove = true;

            return 0;
        }

        LRESULT onExitSizeMove(LPARAM lparam, WPARAM wparam)
        {
            MessageBoxA(nullptr, "4", "onExitSizeMove", 0);

            s_in_sizemove = false;

            RECT rc;
            POINT ptCenter;
            HWND handle_desktop = GetDesktopWindow();
            GetWindowRect(handle_desktop, &rc);
            ptCenter.x = (rc.left + rc.right);
            ptCenter.y = (rc.top + rc.bottom);

            // Here is the other place where you handle the swapchain resize after the user stops using the 'rubber-band' 
            return 0;
        }


        LRESULT onSize(LPARAM lparam, WPARAM wparam)
        {
            using namespace std::chrono_literals;


            if (wparam == SIZE_MINIMIZED)
            {
                MessageBoxA(nullptr, "1", "Minimized", 0);

                if (!s_minimized)
                    s_minimized = true;
            }
            else if (s_minimized)
            {
                MessageBoxA(nullptr, "2", "Restored", 0);

                // The window was minimized and is now restored (resume from suspend)
                s_minimized = false;
            }
            else if (!s_in_sizemove)
            {
                //MessageBoxA(nullptr, "3", "No Size Move", 0);
                // Here is where you handle the swapchain resize for maximize or unmaximize
            }

            return 0;
        }

        LRESULT onPaint(LPARAM lparam, WPARAM wparam)
        {
            PAINTSTRUCT ps{ 0 };
            HDC hdc{ 0 };

            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);

            return 0;
        }

        LRESULT onCreate(LPARAM lparam, WPARAM wparam)
        {
            return 0;
        }

        LRESULT onDestroy(LPARAM lparam, WPARAM wparam)
        {
            PostQuitMessage(0);
            return 0;
        }

        LRESULT onClose(LPARAM lparam, WPARAM wparam)
        {
            DestroyWindow(hwnd);
            return 0;
        }

    private:
        bool s_in_sizemove = false;
        bool s_minimized = false;

        bool is_resizable = true;

    protected:
        WNDCLASSEX winclass;
        HWND winhandle;

        string name, class_name;

        POINT MousePosition;
        MSG msg{ 0 };

        unsigned int width, height;
        bool fullscreen;


        utils::Timer Timer;
        double CurrTime = 0.0;
        double PrevTime = Timer.GetElapsedTime();
        double ElapsedTime = 0.f;

        double filteredFrameTime = 0.0;
    };
}