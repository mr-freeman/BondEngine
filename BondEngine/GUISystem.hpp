// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

#include <ImGuiImplWin32.hpp>

#include <..\DiligentTools\ThirdParty\imgui\imgui.h>

namespace be::gui
{
    using namespace Diligent;

    class GUISystem
    {
    public:
        GUISystem() :
            font_default(nullptr),
            winhandle{ nullptr }
        { };

       ~GUISystem()
        { 
           if (font_default)
               font_default = nullptr;
        };

    public:
        void preinitialize(const math::int2& res, HWND hWnd, IRenderDevice* pDevice);
        void initialize();
        void finalize();

        void newFrame();
        // Every GUI element should be called before this function.
        void render(IDeviceContext* immediate_context);
        void endFrame();

        void loadDefaultFont();

        const unique_ptr<Diligent::ImGuiImplWin32>& getImGUI() const { return imgui; }

    private:
        unique_ptr<Diligent::ImGuiImplWin32> imgui;
        math::int2 resolution;
        HWND winhandle;
        IRenderDevice* device;

        ImFont* font_default;
    };
}