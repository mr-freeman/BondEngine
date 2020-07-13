// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <D3D11Render.hpp>

#include <Error.hpp>
#include <Log.hpp>

namespace be::render
{
    using be::utils::log::info;

    D3D11Render::D3D11Render() :
        clear_color{ 0.0f, 0.0f, 0.0f, 1.0f },
        hwnd{ nullptr }
    {
        name = "Render"s;
        lib_name = "Diligent"s;

        api_name = "Direct3D"s;
        api_version = "11"s;
    }

    void D3D11Render::preinitialize(HWND winhandle, const math::int2& resolution, const bool& fullscreen)
    {
        hwnd = winhandle;

        if (!fullscreen)
            swap_chain_desc_size = resolution;
        else
            for (auto& mode : m_DisplayModes)
                if ((mode.Width == resolution.x) && (mode.Height == resolution.y))
                    swap_chain_desc_size = resolution;

        if(!swap_chain_desc_size.has_value() && fullscreen)
        {
            // keep trying, in order not to disapoint end-users
            RECT rc;
            ::GetWindowRect(::GetDesktopWindow(), &rc);

            swap_chain_desc_size = { rc.right, rc.bottom };

            info("Detect native desktop display mode {}x{}", rc.right, rc.bottom);
        }
    }

    void DiligentMassageCallback(DEBUG_MESSAGE_SEVERITY Severity, const Char* Message, const char* Function, const char* File, int Line)
    {
        switch (Severity)
        {
            case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_INFO:
                info("DiligentEngine: {}", Message);
                return;
            case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_WARNING:
                be::utils::log::warn("DiligentEngine: {}", Message);
                return;
            case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_ERROR:
                be::utils::log::error("DiligentEngine: {}", Message);
                return;
            case DEBUG_MESSAGE_SEVERITY::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR:
                be::utils::log::critical("DiligentEngine: {}", Message);
                return;
            default:
                return;
        };
    }

    void D3D11Render::initialize()
    {
        info("{} initialization is started"s, name);
        info("{} : {} {}"s, lib_name, api_name, api_version);

        if (hwnd == nullptr)
        {
            // Forget to call preinitialize (HWND, math::float2)?
            core::FatalError fe("D3D11Render.cpp", 
                                62, 
                                be::core::Error::Type::GRAPHICS, 
                                "Window handle is invalid");
            fe.finalize();
        }

        auto GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
        pFactoryD3D11 = GetEngineFactoryD3D11();

        // SetDebugMessageCallback(DebugMessageCallback);

#ifdef BE_DEBUG
        EngineCI.DebugMessageCallback = DiligentMassageCallback;
        EngineCI.DebugFlags |= D3D11_DEBUG_FLAG_CREATE_DEBUG_DEVICE;
#endif

        pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &device, &immediate_context);

        SwapChainDesc SCDesc;
        SCDesc.BufferCount = 2;
        
        SCDesc.Width = swap_chain_desc_size.value().x;
        SCDesc.Height = swap_chain_desc_size.value().y;

        SCDesc.ColorBufferFormat = texture_format;
        SCDesc.DepthBufferFormat = TEX_FORMAT_D32_FLOAT;

        SCDesc.Usage = SWAP_CHAIN_USAGE_RENDER_TARGET;
        SCDesc.PreTransform = SURFACE_TRANSFORM_OPTIMAL;
        SCDesc.DefaultDepthValue = 1.0f;
        SCDesc.DefaultStencilValue = 0;
        SCDesc.IsPrimary = True;

        pFactoryD3D11->CreateSwapChainD3D11(device, immediate_context, SCDesc, { }, Win32NativeWindow{ hwnd }, &swap_chain);

        getCaps();
        getDeviceCaps();


        state = State::INITIALIZED;
        info("{} is initialized"s, name);
    }

    void D3D11Render::finalize()
    {
        if (state == State::INITIALIZED)
        {
            if (pRTV)
            {
                pRTV = nullptr;
            }

            if (pDSV)
            {
                pDSV = nullptr;
            }

            if (pipeline_state)
            {
                pipeline_state->Release();
                pipeline_state = nullptr;
            }

            if (immediate_context)
            {
                immediate_context = nullptr;
            }

            if (device)
            {
                device->Release();
                device = nullptr;
            }

            swap_chain->SetWindowedMode();

            if (swap_chain)
            {
                swap_chain->Release();
                swap_chain = nullptr;
            }

            info("{} is finalized"s, name);
        }
    }

    D3D11Render::~D3D11Render()
    {
        // In order to reset render and keep D3D11 loaded.
        if (pFactoryD3D11)
        {
            pFactoryD3D11->Release();
            pFactoryD3D11 = nullptr;
        }
    }

    void D3D11Render::present(const bool& vsync)
    {
        swap_chain->Present(vsync ? 1u : 0u);
    }

    void D3D11Render::render(ITextureView* ms_color_rtv, ITextureView* ms_depth_dsv)
    {
        if ((ms_color_rtv != nullptr) && (ms_depth_dsv != nullptr))
        {
            // Set off-screen multi-sampled render target and depth-stencil buffer
            pRTV = ms_color_rtv;
            pDSV = ms_depth_dsv;
        }
        else
        {
            // Render directly to the current swap chain back buffer.
            pRTV = swap_chain->GetCurrentBackBufferRTV();
            pDSV = swap_chain->GetDepthBufferDSV();
        }

        immediate_context->SetRenderTargets(1u, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        immediate_context->ClearRenderTarget(pRTV, clear_color, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        immediate_context->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0u, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    void D3D11Render::reset()
    {
        this->finalize();
        this->initialize();

        info("{} {} {} is reset"s, lib_name, api_name, api_version);
    }

    void D3D11Render::setPipelineState()
    {
        immediate_context->SetPipelineState(pipeline_state);
    }

    void D3D11Render::resize(const math::int2& resolution)
    {
        if (swap_chain)
            swap_chain->Resize(static_cast<Diligent::Uint32>(resolution.x), 
                               static_cast<Diligent::Uint32>(resolution.y));
    }

    void D3D11Render::setWindowed()
    {
        swap_chain->SetWindowedMode();
    }

    void D3D11Render::setFullScreen(const math::int2& resolution)
    {
        DisplayModeAttribs dma;
        memset(&dma, 0, sizeof(DisplayModeAttribs));

        dma.Width = resolution.x;
        dma.Height = resolution.y;
        dma.RefreshRateNumerator = 60;
        dma.RefreshRateDenominator = 1;
        dma.Format = swap_chain->GetDesc().ColorBufferFormat;
        dma.Scaling = SCALING_MODE::SCALING_MODE_UNSPECIFIED;
        dma.ScanlineOrder = SCANLINE_ORDER::SCANLINE_ORDER_UNSPECIFIED;
        
        assert(m_DisplayModes.size() != 0);

        for (auto& mode : m_DisplayModes)
        {
            if ((mode.Width == dma.Width) && (mode.Height == dma.Height) && (mode.Format == dma.Format))
            {
                current_dma = mode;
                swap_chain->SetFullscreenMode(current_dma);

                uint x = current_dma.Scaling;
                be::utils::log::debug("Apply display mode {}x{}", current_dma.Width, current_dma.Height, x);
                return;
            }
        }

       be::utils::log::warn("Can't apply display mode {}x{}",
                         static_cast<uint>(dma.Width),
                         static_cast<uint>(dma.Height));

       RECT rc;
       ::GetWindowRect(::GetDesktopWindow(), &rc);

       dma.Width = rc.right;
       dma.Height = rc.bottom;

       info("Detect native desktop display mode {}x{}", dma.Width, dma.Height);

       current_dma = dma;
       swap_chain->SetFullscreenMode(current_dma);
    }

    void D3D11Render::createUniBuffer(IBuffer** uniform_buffer, const Diligent::Uint32& buffer_size)
    {
        CreateUniformBuffer(device, buffer_size, "Constants CB", uniform_buffer);
    }

    void D3D11Render::createSRB(IShaderResourceBinding** srb)
    {
        pipeline_state->CreateShaderResourceBinding(srb, true);
    }

    void D3D11Render::createPipelineState()
    {
        device->CreatePipelineState(PSOCreateInfo, &pipeline_state);
    }

    void D3D11Render::getStaVarByName(SHADER_TYPE shader_type, IBuffer* cbuffer, const char* buffer_name)
    {
        pipeline_state->GetStaticVariableByName(shader_type, buffer_name)->Set(cbuffer);
    }

    void D3D11Render::loadTexture(ITexture** Tex, const fs::path& file_name)
    {
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB = (texture_format == TEX_FORMAT_RGBA8_UNORM_SRGB ? True : False);

        CreateTextureFromFile(file_name.string().c_str(), loadInfo, device, Tex);
    }
    void D3D11Render::getCaps()
    {
        Uint32 NumAdapters = 0;
        pFactoryD3D11->EnumerateAdapters(EngineCI.MinimumFeatureLevel, NumAdapters, 0);
        vector<AdapterAttribs> Adapters(NumAdapters);

        if (NumAdapters > 0)
            pFactoryD3D11->EnumerateAdapters(EngineCI.MinimumFeatureLevel, NumAdapters, Adapters.data());

        if (adapter_type == ADAPTER_TYPE_SOFTWARE)
            for (auto& adapter : Adapters)
                if (adapter.AdapterType == adapter_type)
                {
                    info("Found software adapter: {}", adapter.Description);

                    break;
                }

        m_AdapterAttribs = Adapters[m_AdapterId];
        if (adapter_type != ADAPTER_TYPE_SOFTWARE)
        {
            Uint32 NumDisplayModes = 0;
            pFactoryD3D11->EnumerateDisplayModes(EngineCI.MinimumFeatureLevel, m_AdapterId, 0, texture_format, NumDisplayModes, nullptr);
            m_DisplayModes.resize(NumDisplayModes);
            pFactoryD3D11->EnumerateDisplayModes(EngineCI.MinimumFeatureLevel, m_AdapterId, 0, texture_format, NumDisplayModes, m_DisplayModes.data());
        }

        EngineCI.AdapterId = m_AdapterId;

        assert(Adapters.size());
    }

    void D3D11Render::getDeviceCaps()
    {
        adapter_type = device->GetDeviceCaps().AdaterType;

        if (adapter_type == ADAPTER_TYPE::ADAPTER_TYPE_HARDWARE)
            info("Hardware rendering is supported"s);

        auto aniso_supported = device->GetDeviceCaps().SamCaps.AnisotropicFilteringSupported;
        info("Anisotropic Filtering is {}supported"s, aniso_supported ? "" : "not ");

        auto tesselation_supported = device->GetDeviceCaps().Features.Tessellation;
        info("Tesselation is {}supported"s, tesselation_supported ? "" : "not ");

        for (auto& attr : Adapters)
        {
            info("Device ID: {}", attr.DeviceId);
            info("Device Outputs: {}", attr.NumOutputs);

            string vendor = ""s;
            if (attr.VendorId & 0x1002)
                vendor = "AMD"s;
            else if (attr.VendorId & 0x10DE)
                vendor = "Nvidia"s;
            else if ((attr.VendorId & 0x8086) ||
                     (attr.VendorId & 0x8087) ||
                     (attr.VendorId & 0x163C))
                vendor = "Intel"s;
            else
                vendor = "Unknown"s;

            info("Device Vendor: {}", vendor);
            info("Device Brand: {}", attr.Description);

            auto shared_sys_mem = attr.SharedSystemMemory / 1024 / 1024;
            if (shared_sys_mem != 0)
                info("Device Shared System Memory: {} MB", shared_sys_mem);

            auto dedicated_sys_mem = attr.DedicatedSystemMemory / 1024 / 1024;
            if (dedicated_sys_mem != 0)
                info("Device Dedicated System Memory: {} MB", dedicated_sys_mem);

            auto dedicated_vid_mem = attr.DedicatedVideoMemory / 1024 / 1024;
            if (dedicated_vid_mem != 0)
                info("Device Dedicated Video Memory: {} MB", dedicated_vid_mem);
        }
    }
}