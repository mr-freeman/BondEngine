// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <IRender.hpp>

namespace be::render::utils
{
    struct Vertex
    {
        Vertex(float x, float y, float z,
               float u, float v,
               float nx, float ny, float nz)
            : pos(x, y, z), uv(u, v), normal(nx, ny, nz)
        { }

        math::float3 pos;
        math::float2 uv;
        math::float3 normal;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
       ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;

        VertexBuffer operator=(const VertexBuffer&) = delete;
        VertexBuffer operator=(VertexBuffer&&) = delete;

    public:
        void create(const shared_ptr<render::IRender>& rend, Vertex Verteces[], const Uint32& size);
        void render(const shared_ptr<render::IRender>& rend);

        IBuffer** get() { return &vertex_buffer; }

    public:
        IBuffer* vertex_buffer = nullptr;

        BufferData VBData;
        BufferDesc VertBuffDesc;

        Uint32 offset = 0;
        string name = ""s;
        size_t count = 0;
    };
}