// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>

#include <..\DiligentTools\TextureLoader\interface\TextureUtilities.h>
#include <..\DiligentCore\Graphics\GraphicsTools\interface\MapHelper.hpp>
#include <..\DiligentCore\Graphics\GraphicsTools\interface\GraphicsUtilities.h>

namespace be::render
{
    // Direct3D11 implementation using Diligent Graphics Engine.
    class D3D11Render : virtual public IRender
    {
    public:
        D3D11Render();
       ~D3D11Render();

        D3D11Render(const D3D11Render&) = delete;
        D3D11Render(D3D11Render&&) = delete;

        D3D11Render operator=(const D3D11Render&) = delete;
        D3D11Render operator=(D3D11Render&&) = delete;

    public:
        void preinitialize(HWND winhandle, const math::int2& resolution, const bool& fullscreen) override final;
        void initialize() override final;
        void run() override final { }; // use present instead
        void finalize() override final;

        void present(const bool& vsync) override final;
        void render(ITextureView* ms_color_rtv = nullptr, ITextureView* ms_depth_dsv = nullptr) override final;
        void reset() override final;


        IEngineFactory* getEngineFactory() override final { return pFactoryD3D11; }
        IRenderDevice* getRenderDevice() override final { return device; }
        IDeviceContext* getDeviceContext() override final { return immediate_context; }
        ISwapChain* getSwapChain() override final { return swap_chain; }
        IPipelineState* getPipelineState() override final { return pipeline_state; }
        PipelineStateCreateInfo& getPipelineInfo() override final { return *&PSOCreateInfo; }

    public:
        void getCaps() override final;
        void getDeviceCaps() override final;
        void setPipelineState() override final;
        void setFullScreen(const math::int2& resolution) override final;
        void setWindowed() override final;
        void resize(const math::int2& resolution);

        void createPipelineState() override final;
        void createSRB(IShaderResourceBinding** srb) override final;
        void createUniBuffer(IBuffer** VSConstants, const Diligent::Uint32& size) override final;
        void loadTexture(ITexture** Tex, const fs::path& file_name) override final;
        void getStaVarByName(SHADER_TYPE sht, IBuffer* VSConstants, const char* cb_name) override final;

    private:
        IEngineFactoryD3D11* pFactoryD3D11 = nullptr;
        IRenderDevice* device = nullptr;
        IDeviceContext* immediate_context = nullptr;
        ISwapChain* swap_chain = nullptr;
        IPipelineState* pipeline_state = nullptr;

        ITextureView* pRTV = nullptr;
        ITextureView* pDSV = nullptr;

    public:
        EngineD3D11CreateInfo EngineCI;
        PipelineStateCreateInfo PSOCreateInfo;

        vector<AdapterAttribs> Adapters;

        TEXTURE_FORMAT texture_format = TEX_FORMAT_RGBA8_UNORM_SRGB;
        RENDER_DEVICE_TYPE m_DeviceType = RENDER_DEVICE_TYPE_D3D11;
        ADAPTER_TYPE adapter_type = ADAPTER_TYPE_UNKNOWN;

        AdapterAttribs m_AdapterAttribs;
        vector<DisplayModeAttribs> m_DisplayModes;
        DisplayModeAttribs current_dma;

        HWND hwnd;
        std::optional<math::int2> swap_chain_desc_size;

        float clear_color[4];
        Uint32 m_AdapterId = 0;
    };
}