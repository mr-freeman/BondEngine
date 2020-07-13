// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <..\DiligentCore\Graphics\GraphicsEngine\interface\GraphicsTypes.h>
#include <..\DiligentCore\Common\interface\BasicMath.hpp>
#include <..\DiligentCore\Common\interface\AdvancedMath.hpp>

#include <IGameObject.hpp>

#include <InputHandler.hpp>

namespace be::game
{
    using namespace Diligent;
    using namespace math;

    class FPSCamera : public IGameObject
    {
    public:
        FPSCamera();
       ~FPSCamera() = default;

        FPSCamera(const FPSCamera&) = delete;
        FPSCamera(FPSCamera&&) = delete;

        FPSCamera operator=(const FPSCamera&) = delete;
        FPSCamera operator=(FPSCamera&&) = delete;

    public:
        void update(const float& curr, const float& elapsed_time, gainput::InputMap& map);

        void setPosition(const float3& pos);
        void setLookAt(const float3& LookAt);
        void setRotation(float Yaw, float Pitch);
        void setProjAttribs(Float32           NearClipPlane,
                            Float32           FarClipPlane,
                            Float32           AspectRatio,
                            Float32           FOV,
                            SURFACE_TRANSFORM SrfPreTransform,
                            bool              IsGL);

        float4x4 getReferenceRotation();
        float4x4& getWVP() { return m_WorldMatrix; }

        float3 getPosition() const { return position; }
        float2 getLookAt() const { return { m_fPitchAngle, m_fYawAngle }; }

        float GetDeltaX() const { return delta_x; }
        float GetDeltaY() const { return delta_y; }

    public:
        struct ProjectionAttribs
        {
            Float32           NearClipPlane = 1.f;
            Float32           FarClipPlane = 1000.f;
            Float32           AspectRatio = 1366.f / 768.f;
            Float32           FOV = PI_F / 4.f;
            SURFACE_TRANSFORM PreTransform = SURFACE_TRANSFORM_IDENTITY;
            bool              IsGL = false; // Who wants OpenGL?
        };

    private:
        ProjectionAttribs m_ProjAttribs;

        float2 m_MouseState;
        float2 m_LastMouseState;

        std::pair<uint, uint> center_resolution;
        POINT p;

        float3 m_ReferenceRightAxis = float3{ 1, 0, 0 };
        float3 m_ReferenceUpAxis = float3{ 0, 1, 0 };
        float3 m_ReferenceAheadAxis = float3{ 0, 0, 1 };

        float3 position = float3{ 0, 0, 5 };

        float4x4 m_ViewMatrix;
        float4x4 m_WorldMatrix;
        float4x4 m_ProjMatrix;

    public:
        float m_fov = 75.f; // default
        float m_fRotationSpeed = 0.001f;
        float m_fMoveSpeed = .01f;
        float m_fCurrentSpeed = 0.f;

        float m_fYawAngle = 0;
        float m_fPitchAngle = 0;
        float m_fSpeedUpScale = 1.f;
        float m_fSuperSpeedUpScale = 1.f;
        float m_fHandness = 1.f; // -1 - left handed
    };
}