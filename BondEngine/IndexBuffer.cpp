// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <IndexBuffer.hpp>

namespace be::render::utils
{
    IndexBuffer::~IndexBuffer()
    {
        if (index_buffer)
        {
            index_buffer->Release();
            index_buffer = nullptr;
        }
    }

    void IndexBuffer::create(const shared_ptr<render::IRender>& rend, Uint32 Indices[], const Uint32& size)
    {
        IndBuffDesc.Name = "Cube index buffer";
        IndBuffDesc.Usage = USAGE_STATIC;
        IndBuffDesc.BindFlags = BIND_INDEX_BUFFER;
        IndBuffDesc.uiSizeInBytes = size;

        IBData.pData = Indices;
        IBData.DataSize = size;
        rend->getRenderDevice()->CreateBuffer(IndBuffDesc, &IBData, &index_buffer);
    }

    void IndexBuffer::render(const shared_ptr<render::IRender>& rend)
    {
        rend->getDeviceContext()->SetIndexBuffer(index_buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }
}