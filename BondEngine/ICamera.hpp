// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <..\DiligentCore\Common\interface\BasicMath.hpp>

namespace be::game
{
    using namespace Diligent::Math;

    class ICamera
    {
    public:
                 ICamera() = default;
        virtual ~ICamera() = default;

        ICamera(const ICamera&) = default;
        ICamera(ICamera&&) = default;

        ICamera operator=(const ICamera&) = delete;
        ICamera operator=(ICamera&&) = delete;

    public:
        virtual void update(const float& current_time, const float& elapsed_time, gainput::InputMap& map) = 0;

        virtual void setPosition(const float3& pos) = 0;
        virtual void setLookAt(const float3& LookAt) = 0;
        virtual void setProjectionDesc(float           NearClipPlane,
                                       float           FarClipPlane,
                                       float           AspectRatio,
                                       float           FOV,
                                       bool              IsGL,
                                       Diligent::SURFACE_TRANSFORM SrfPreTransform) = 0;

        virtual float3 getPosition() const = 0;
        virtual float2 getLookAt() const = 0;

        virtual float4x4 getReferenceRotation() const = 0;
    };
}