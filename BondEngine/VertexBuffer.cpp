// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <VertexBuffer.hpp>

#include <Log.hpp>

namespace be::render::utils
{
    VertexBuffer::~VertexBuffer()
    {
        if (vertex_buffer)
        {
            vertex_buffer->Release();
            vertex_buffer = nullptr;
        }
    }

    void VertexBuffer::create(const shared_ptr<render::IRender>& rend, Vertex Verteces[], const Uint32& size)
    {
        VertBuffDesc.Name = "Cube vertex buffer";
        VertBuffDesc.Usage = USAGE_STATIC;
        VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
        VertBuffDesc.uiSizeInBytes = size;

        VBData.pData = Verteces;
        VBData.DataSize = size;
        rend->getRenderDevice()->CreateBuffer(VertBuffDesc, &VBData, &vertex_buffer);
    }

    void VertexBuffer::render(const shared_ptr<render::IRender>& rend)
    {
        rend->getDeviceContext()->SetVertexBuffers(0, 1, &vertex_buffer, &offset,
                                                   RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
                                                   SET_VERTEX_BUFFERS_FLAG_RESET);
    }
}