// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <SimpleCube.hpp>

#include <Log.hpp>
#include <FileSystem.hpp>

namespace be::game
{
    SimpleCube::SimpleCube() :
        vertex_buffer(make_unique<render::utils::VertexBuffer>()),
        index_buffer(make_unique<render::utils::IndexBuffer>()),
        vertex_shader(make_unique<render::utils::VertexShader>()),
        pixel_shader(make_unique<render::utils::PixelShader>()),
        geometry_shader(make_unique<render::utils::GeometryShader>())
    { }

    SimpleCube::~SimpleCube()
    {
        if (shader_res_bind)
        {
            shader_res_bind->Release();
            shader_res_bind = nullptr;
        }

        if (constant_buffer)
        {
            constant_buffer->Release();
            constant_buffer = nullptr;
        }
    }

    void SimpleCube::initialize()
    {
        using namespace Diligent;

        light.direction = math::float4(0.0f, 0.5f, -0.2f, 0.f);
        light.ambient = math::float4(1.0f, 1.0f, 1.0f, 1.0f);
        light.diffuse = math::float4(1.0f, 0.5f, 0.5f, 1.0f);


        Uint32 Indices[] =
        {
            2,0,1,    2,3,0,
            4,6,5,    4,7,6,
            8,10,9,   8,11,10,
            12,14,13, 12,15,14,
            16,18,17, 16,19,18,
            20,21,22, 20,22,23
        };

        index_buffer->create(rend, Indices, sizeof(Indices));

        using math::float2;
        using math::float3;

        render::utils::Vertex CubeVerts[] =
        {
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f),
            Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f,  1.0f, -1.0f),
            Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
            Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

            Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
            Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
            Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
            Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

            Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
            Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
            Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
            Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

            Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
            Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
            Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

            Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
            Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
            Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
            Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
            Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
            Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
            Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
        };

        vertex_buffer->create(rend, CubeVerts, sizeof(CubeVerts));

        //--------------
        auto& PSODesc = rend->getPipelineInfo().PSODesc;

        PSODesc.Name = "Textured Cube PSO";
        PSODesc.IsComputePipeline = false;

        PSODesc.GraphicsPipeline.NumRenderTargets = 1;
        PSODesc.GraphicsPipeline.RTVFormats[0] = Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB;
        PSODesc.GraphicsPipeline.DSVFormat = rend->getSwapChain()->GetDesc().DepthBufferFormat;
        PSODesc.GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSODesc.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
        PSODesc.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        rend->createUniBuffer(&constant_buffer_light, sizeof(ConstantsLight));
        rend->createUniBuffer(&constant_buffer, sizeof(Constants));

        Diligent::ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.UseCombinedTextureSamplers = true;

        Diligent::RefCntAutoPtr<Diligent::IShaderSourceInputStreamFactory> shader_source_fact;
        rend->getEngineFactory()->CreateDefaultShaderSourceStreamFactory(nullptr, &shader_source_fact);
        ShaderCI.pShaderSourceStreamFactory = shader_source_fact;

        vertex_shader->open("light_vs"s);
        vertex_shader->create(rend, ShaderCI);

        pixel_shader->open("light_ps"s);
        pixel_shader->create(rend, ShaderCI);

        Diligent::LayoutElement LayoutElems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False}, // Attribute 0 - vertex position
            LayoutElement{1, 0, 2, VT_FLOAT32, False}, // Attribute 1 - texture coordinates
            LayoutElement{2, 0, 3, VT_FLOAT32, True}, // Attribute 2 - normal coordinates
        };

        PSODesc.GraphicsPipeline.pVS = vertex_shader->get();
        PSODesc.GraphicsPipeline.pPS = pixel_shader->get();

        PSODesc.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
        PSODesc.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(std::size(LayoutElems));

        PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        // Shader variables should typically be mutable, which means they are expected
        // to change on a per-instance basis
        ShaderResourceVariableDesc Vars[] =
        {
            { SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE }
        };

        PSODesc.ResourceLayout.Variables = Vars;
        PSODesc.ResourceLayout.NumVariables = static_cast<Uint32>(std::size(Vars));

        // Define static sampler for g_Texture. Static samplers should be used whenever possible.
        auto anisotropic_filtering = true;
        auto filtering_type = anisotropic_filtering ? FILTER_TYPE_COMPARISON_ANISOTROPIC : FILTER_TYPE_LINEAR;
        auto comparision = anisotropic_filtering ? COMPARISON_FUNC_ALWAYS : COMPARISON_FUNC_NEVER;
        Uint32 anisotropic_sample_count = 16u;

        SamplerDesc SamLinearClampDesc
        {
            filtering_type, filtering_type, filtering_type,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, 0, anisotropic_sample_count, comparision
        };

        StaticSamplerDesc StaticSamplers[] =
        {
            StaticSamplerDesc{ SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc }
        };

        PSODesc.ResourceLayout.StaticSamplers = StaticSamplers;
        PSODesc.ResourceLayout.NumStaticSamplers = static_cast<Uint32>(std::size(StaticSamplers));

        rend->createPipelineState();

        rend->getStaVarByName(SHADER_TYPE_VERTEX, constant_buffer, "Constants");
        rend->getStaVarByName(SHADER_TYPE_PIXEL, constant_buffer_light, "ConstantsLight");

        rend->createSRB(&shader_res_bind);
    }

    void SimpleCube::loadTexture()
    {
        const auto cube_texture_name = "box"s;
        const auto JPG_EXTENSION = ".jpg"s;

        texture = make_unique<UnifiedTextureFile>(cube_texture_name, JPG_EXTENSION);
        texture->load(texture->full_name, rend->getRenderDevice());

        shader_res_bind->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "g_Texture")->Set(texture->getTextureView());
    }

    void SimpleCube::render(math::float4x4& wvp)
    {
        using namespace Diligent;

        MapHelper<ConstantsLight> CBConstants2(rend->getDeviceContext(), constant_buffer_light, MAP_WRITE, MAP_FLAG_DISCARD);
        CBConstants2->light = light;

        MapHelper<Constants> CBConstants(rend->getDeviceContext(), constant_buffer, MAP_WRITE, MAP_FLAG_DISCARD);
        CBConstants->WorldViewProj = wvp.Transpose();
        CBConstants->World = CBConstants->WorldViewProj;
        

        vertex_buffer->render(rend);
        index_buffer->render(rend);

        rend->setPipelineState();
        rend->getDeviceContext()->CommitShaderResources(shader_res_bind, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;
        DrawAttrs.IndexType = VT_UINT32;
        DrawAttrs.NumIndices = 36u;
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;

        rend->getDeviceContext()->DrawIndexed(DrawAttrs);
    }

    void SimpleCube::update(math::float4x4& wvp, const float& current)
    {
        //math::float4x4 CubeModelTransform = math::float4x4::RotationY(current * 1.0f);

        //wvp *= CubeModelTransform; //* CubeModelTransform;
    }
}