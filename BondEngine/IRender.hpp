// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IEngine.hpp>

#include <Common/interface/RefCntAutoPtr.hpp>

#include <..\DiligentCore\Graphics\GraphicsTools\interface\GraphicsUtilities.h>
#include <..\DiligentCore\Graphics\GraphicsTools\interface\MapHelper.hpp>
#include <..\DiligentTools\TextureLoader\interface\TextureUtilities.h>
#include <..\DiligentCore\Common\interface\BasicMath.hpp>

namespace be::render
{
    class IRender : public IEngine, public IEngine::Component
    {
    public:
        IRender() = default;
        virtual ~IRender() = default;

        IRender(const IRender&) = delete;
        IRender(IRender&&) = delete;

        IRender operator=(const IRender&) = delete;
        IRender operator=(IRender&&) = delete;

    public:
        virtual void preinitialize(HWND winhandle, const math::int2& resolution, const bool& fullscreen) = 0;
        virtual void initialize() = 0;
        virtual void run() = 0;
        virtual void finalize() = 0;

        virtual void present(const bool& vsync) = 0;
        virtual void render(ITextureView* ms_color_rtv = nullptr, ITextureView* ms_depth_dsv = nullptr) = 0;
        virtual void reset() = 0;

        virtual void getCaps() = 0;
        virtual void getDeviceCaps() = 0;
        virtual void setFullScreen(const math::int2& resolution) = 0;
        virtual void setWindowed() = 0;

        virtual IEngineFactory* getEngineFactory() = 0;
        virtual IRenderDevice* getRenderDevice() = 0;
        virtual IDeviceContext* getDeviceContext() = 0;
        virtual ISwapChain* getSwapChain() = 0;
        virtual IPipelineState* getPipelineState() = 0;

        virtual void getStaVarByName(SHADER_TYPE sht, IBuffer* VSConstants, const char* buffer_name) = 0;

        virtual void createPipelineState() = 0;
        virtual PipelineStateCreateInfo& getPipelineInfo() = 0;
        virtual void setPipelineState() = 0;

        virtual void createSRB(IShaderResourceBinding** srb) = 0;
        virtual void createUniBuffer(IBuffer** VSConstants, const Uint32& size) = 0;
        virtual void loadTexture(ITexture** Tex, const fs::path& file_name) = 0;
    };
}