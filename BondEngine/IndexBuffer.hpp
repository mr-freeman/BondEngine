// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>

namespace be::render::utils
{
    // REDO: Buffers, AA, GUI, Light, Effect, Shader should not be in be::render namespace.
    //       Place them into graphics/engine etc namespace.
    class IndexBuffer
    {
    public:
        IndexBuffer() = default;
       ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = delete;

        IndexBuffer operator=(const IndexBuffer&) = delete;
        IndexBuffer operator=(IndexBuffer&&) = delete;

    public:
        void create(const shared_ptr<render::IRender>& rend, Uint32 Indices[], const Uint32& size);
        void render(const shared_ptr<render::IRender>& rend);

        IBuffer* get() { return index_buffer; }

    public:
        IBuffer* index_buffer = nullptr;

        BufferData IBData;
        BufferDesc IndBuffDesc;

        string name = ""s;
        size_t count = 0;
    };
}