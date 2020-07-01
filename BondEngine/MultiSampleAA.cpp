// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <MultiSampleAA.hpp>

#include <Log.hpp>

namespace be::render
{
    void MultiSampleAA::createRenderTarget(const uint& sample_count)
    {
        const auto& SCDesc = rend->getSwapChain()->GetDesc();
        // Create window-size multi-sampled offscreen render target
        TextureDesc ColorDesc;
        ColorDesc.Name = "MultiSampled Render Target";
        ColorDesc.Type = RESOURCE_DIM_TEX_2D;
        ColorDesc.BindFlags = BIND_RENDER_TARGET;
        ColorDesc.Width = SCDesc.Width;
        ColorDesc.Height = SCDesc.Height;
        ColorDesc.MipLevels = 1;
        ColorDesc.Format = SCDesc.ColorBufferFormat;

        bool NeedsSRGBConversion = rend->getRenderDevice()->GetDeviceCaps().IsD3DDevice() && (ColorDesc.Format == TEX_FORMAT_RGBA8_UNORM_SRGB || ColorDesc.Format == TEX_FORMAT_BGRA8_UNORM_SRGB);
        if (NeedsSRGBConversion)
        {
            // Internally Direct3D swap chain images are not SRGB, and ResolveSubresource
            // requires source and destination formats to match exactly or be typeless.
            // So we will have to create a typeless texture and use SRGB render target view with it.
            ColorDesc.Format = ColorDesc.Format == TEX_FORMAT_RGBA8_UNORM_SRGB ? TEX_FORMAT_RGBA8_TYPELESS : TEX_FORMAT_BGRA8_TYPELESS;
        }

        // Set the desired number of samples
        ColorDesc.SampleCount = (supported_sample_counts != sample_count) ? supported_sample_counts : sample_count;

        // Define optimal clear value
        ColorDesc.ClearValue.Format = SCDesc.ColorBufferFormat;
        ColorDesc.ClearValue.Color[0] = 0.125f;
        ColorDesc.ClearValue.Color[1] = 0.125f;
        ColorDesc.ClearValue.Color[2] = 0.125f;
        ColorDesc.ClearValue.Color[3] = 1.f;
        ITexture* pColor = nullptr;
        rend->getRenderDevice()->CreateTexture(ColorDesc, nullptr, &pColor);

        // Store the render target view
        if (NeedsSRGBConversion)
        {
            TextureViewDesc RTVDesc;
            RTVDesc.ViewType = TEXTURE_VIEW_RENDER_TARGET;
            RTVDesc.Format = SCDesc.ColorBufferFormat;
            pColor->CreateView(RTVDesc, &m_pMSColorRTV);
        }
        else
            m_pMSColorRTV = pColor->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET);

        if (pColor)
        {
            pColor->Release();
            pColor = nullptr;
        }

        // Create window-size multi-sampled depth buffer
        TextureDesc DepthDesc = ColorDesc;
        DepthDesc.Name = "Multisampled depth buffer";
        DepthDesc.Format = DepthBufferFormat;
        DepthDesc.BindFlags = BIND_DEPTH_STENCIL;

        // Define optimal clear value
        DepthDesc.ClearValue.Format = DepthDesc.Format;
        DepthDesc.ClearValue.DepthStencil.Depth = 1;
        DepthDesc.ClearValue.DepthStencil.Stencil = 0;

        ITexture* pDepth = nullptr;
        rend->getRenderDevice()->CreateTexture(DepthDesc, nullptr, &pDepth);
        // Store the depth-stencil view
        m_pMSDepthDSV = pDepth->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);

        if (pDepth)
        {
            pDepth->Release();
            pDepth = nullptr;
        }

    }

    void MultiSampleAA::determine()
    {
        const auto& ColorFmtInfo = rend->getRenderDevice()->GetTextureFormatInfoExt(rend->getSwapChain()->GetDesc().ColorBufferFormat);
        const auto& DepthFmtInfo = rend->getRenderDevice()->GetTextureFormatInfoExt(DepthBufferFormat);
        auto ssc = ColorFmtInfo.SampleCounts & DepthFmtInfo.SampleCounts;

        if (ssc & 0x08)
            supported_sample_counts = 8;
        else if (ssc & 0x04)
            supported_sample_counts = 4;
        else if (ssc & 0x02)
            supported_sample_counts = 2;
        else
        {
            be::utils::log::warn("{} and {} pair does not allow multisampling on this device"s, ColorFmtInfo.Name, DepthFmtInfo.Name);
            supported_sample_counts = 1;
        }

        if(supported_sample_counts > 1)
            be::utils::log::info("MSAA x{} is supported", supported_sample_counts);
    }

    void MultiSampleAA::render()
    {
        // Resolve multi-sampled render taget into the current swap chain back buffer.
        auto pCurrentBackBuffer = rend->getSwapChain()->GetCurrentBackBufferRTV()->GetTexture();

        ResolveTextureSubresourceAttribs ResolveAttribs;
        ResolveAttribs.SrcTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
        ResolveAttribs.DstTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
        rend->getDeviceContext()->ResolveTextureSubresource(m_pMSColorRTV->GetTexture(), pCurrentBackBuffer, ResolveAttribs);
    }
}