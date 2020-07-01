// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <GUISystem.hpp>
#include <Log.hpp>

namespace be::gui
{
    void GUISystem::preinitialize(const math::int2& res, HWND hWnd, IRenderDevice* pDevice)
    { 
        resolution = res;
        winhandle = hWnd;
        device = pDevice;
    }

    void GUISystem::initialize()
    {
        utils::log::info("GUI System initialization is started");
        utils::log::info("ImGUI : 1.77");

        imgui = std::make_unique<Diligent::ImGuiImplWin32>(winhandle,
                                                           device,
                                                           Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB,
                                                           Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB);
        ImGui::CreateContext();
        imgui->CreateDeviceObjects();
        imgui->InvalidateDeviceObjects();
        utils::log::debug("Eventually I have run Diligent Graphics Engine with ImGUI (after one day of debugging)"s); // 12.05.2020 21:35

        // TODO: Load style/gui elements' size from game configs.
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);

        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);

        style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);

        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);

        style.WindowBorderSize = 0.f;
        style.WindowRounding = 0.f;
        style.ScrollbarRounding = 1.f;

        style.ItemSpacing.y = 6.f;

        utils::log::info("GUI System is initialized");
    }

    void GUISystem::finalize()
    {
        utils::log::info("GUI System is finalized");
    }

    void GUISystem::newFrame()
    {
        imgui->NewFrame(resolution.x, resolution.y, Diligent::SURFACE_TRANSFORM_IDENTITY);
        ImGui::PushFont(font_default);
    }

    void GUISystem::render(IDeviceContext* immediate_context)
    {
        ImGui::PopFont();
        imgui->Render(immediate_context);
    }

    void GUISystem::endFrame()
    {
        imgui->EndFrame();
    }

    void GUISystem::loadDefaultFont()
    {
        auto fonts_dir = fs::path("game//fonts//");
         
        auto  font_default_full_name = "game//fonts//default.ttf"s;

        if (!fs::exists(font_default_full_name))
        {
            font_default_full_name = "game\\fonts\\default.ttf"s;
            utils::log::warn("Can't open font file {}", font_default_full_name);
        }
        
        auto& io = ImGui::GetIO();
        font_default = io.Fonts->AddFontFromFileTTF(font_default_full_name.c_str(), 18.f);
        io.Fonts->Build();

        utils::log::info("Open font file {}", font_default_full_name);
    }
}