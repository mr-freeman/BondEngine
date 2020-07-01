// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>

namespace be::render
{
    class MultiSampleAA
    {
    public:
        MultiSampleAA() = default;
       ~MultiSampleAA() = default;

        MultiSampleAA(const MultiSampleAA&) = delete;
        MultiSampleAA(MultiSampleAA&&) = delete;

        MultiSampleAA operator=(const MultiSampleAA&) = delete;
        MultiSampleAA operator=(MultiSampleAA&&) = delete;

    public:
        void determine();
        void createRenderTarget(const uint& sample_count);
        void render();

        const uint& getSupportedSampleCount() { return supported_sample_counts; }
        void setRenderer(const shared_ptr<IRender>& render) { rend = render; }

        ITextureView* getRTV() { return m_pMSColorRTV; }
        ITextureView* getDSV() { return m_pMSDepthDSV; }

    private:
        shared_ptr<IRender> rend;

        static constexpr TEXTURE_FORMAT DepthBufferFormat = TEX_FORMAT_D32_FLOAT;

        RefCntAutoPtr<ITextureView> m_pMSColorRTV;
        RefCntAutoPtr<ITextureView> m_pMSDepthDSV;

        uint supported_sample_counts = 1;
    };
}