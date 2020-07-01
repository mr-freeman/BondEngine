// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <FPSCamera.hpp>

namespace be::game
{
    FPSCamera::FPSCamera()
    {
        RECT rc;
        POINT ptCenter;
        HWND handle_desktop = ::GetDesktopWindow();
        ::GetWindowRect(handle_desktop, &rc);
        ptCenter.x = (rc.left + rc.right) / 2;
        ptCenter.y = (rc.top + rc.bottom) / 2;

        center_resolution = std::make_pair(ptCenter.x, ptCenter.y);
    }

    void FPSCamera::update(const float& curr, const float& elapsed_time, gainput::InputMap& map)
    {
        using be::input::GameButton;

        POINT MousePosition;
        ::GetCursorPos(&MousePosition);
        ::ScreenToClient(::GetActiveWindow(), &MousePosition);

        m_MouseState.x = MousePosition.x;
        m_MouseState.y = MousePosition.y;

        auto MoveDirection = float3(0, 0, 0);

        if (map.GetBool(GameButton::MoveBackward))
            MoveDirection.z += 0.15f;
        if (map.GetBool(GameButton::MoveForward))
            MoveDirection.z -= 0.15f;

        if (map.GetBool(GameButton::MoveRight))
            MoveDirection.x -= 0.15f;
        if (map.GetBool(GameButton::MoveLeft))
            MoveDirection.x += 0.15f;

        // Normalize vector so if moving in 2 dirs (left & forward),
        // the camera doesn't move faster than if moving in 1 dir
        auto len = length(MoveDirection);
        if (len != 0.0)
            MoveDirection /= len;

        bool IsSpeedUpScale = map.GetBool(GameButton::Sprint);
        bool IsSuperSpeedUpScale = map.GetBool(GameButton::Crouch);

        MoveDirection *= m_fMoveSpeed;

        if (IsSpeedUpScale)
            MoveDirection *= m_fSpeedUpScale;
        if (IsSuperSpeedUpScale)
            MoveDirection /= m_fSuperSpeedUpScale;

        m_fCurrentSpeed = length(MoveDirection);

        float3 PosDelta = MoveDirection * elapsed_time; // * elapsed_time

        // GaInput is weak at managing mouse for fps camera,
        // mouse x asix does not return respective delta value the same as y mouse asix
        // So we use external Win32.

        auto MouseDeltaX = 0.f;
        auto MouseDeltaY = 0.f;
        
        MouseDeltaX += m_MouseState.x - m_LastMouseState.x;
        MouseDeltaY += m_MouseState.y - m_LastMouseState.y;

        m_LastMouseState = m_MouseState;

        auto fYawDelta = MouseDeltaX * m_fRotationSpeed;
        auto fPitchDelta = MouseDeltaY * m_fRotationSpeed;

        // In order ::SetCursorPos to work fine it takes vsync to be switched off.
        ::GetCursorPos(&p);

        // BUG: Works only when vsync is turned off.
        if (p.x != center_resolution.first || p.y != center_resolution.second)
        {
            m_fYawAngle += fYawDelta * -m_fHandness;
            m_fPitchAngle += fPitchDelta * -m_fHandness;
            m_fPitchAngle = std::max(m_fPitchAngle, -PI_F / 2.f);
            m_fPitchAngle = std::min(m_fPitchAngle, +PI_F / 2.f);

            ::SetCursorPos(center_resolution.first, center_resolution.second);
        }

        auto ReferenceRotation = getReferenceRotation();

        auto rotation_arb_yaw_up = float4x4::RotationArbitrary(m_ReferenceUpAxis, m_fYawAngle) * ReferenceRotation;
        auto rotation_arb_pitch_right = float4x4::RotationArbitrary(m_ReferenceRightAxis, m_fPitchAngle) * ReferenceRotation;

        auto CameraRotation = rotation_arb_yaw_up * rotation_arb_pitch_right;
        auto WorldRotation = CameraRotation.Transpose();

        auto PosDeltaWorld = PosDelta * WorldRotation;
        position += PosDeltaWorld;

        //-----------------------------------------------------------------
        auto GetAdjustedProjectionMatrix = [&](float FOV, float NearPlane, float FarPlane)
        {
            // const auto& SCDesc = rend->getSwapChain()->GetDesc();

            auto AspectRatio = static_cast<float>(1366) / static_cast<float>(768);
            auto XScale = 0.f, YScale = 0.f;

            YScale = 1.f / std::tan(FOV / 2.f);
            XScale = YScale / AspectRatio;

            float4x4 Proj;
            Proj._11 = XScale;
            Proj._22 = YScale;
            Proj.SetNearFarClipPlanes(NearPlane, FarPlane, false);

            return Proj;
        };

        auto CubeModelTransform = float4x4::RotationY(2.f * elapsed_time); // Apply rotation

        // Get pretransform matrix that rotates the scene according the surface orientation
        auto SrfPreTransform = float4x4::Identity();

        auto calcFOV = [](const float& fov)
        {
            return (fov * 0.01f);
        };

        // Get projection matrix adjusted to the current screen orientation
        m_ProjMatrix = GetAdjustedProjectionMatrix(calcFOV(75), 0.1f, 1000.f);
        m_ViewMatrix = float4x4::Translation(position) * CameraRotation;

        m_WorldMatrix = CubeModelTransform * SrfPreTransform * m_ViewMatrix * m_ProjMatrix;
    }

    float4x4 FPSCamera::getReferenceRotation()
    {
        return float4x4
        {
            m_ReferenceRightAxis.x, m_ReferenceUpAxis.x, m_ReferenceAheadAxis.x, 0,
            m_ReferenceRightAxis.y, m_ReferenceUpAxis.y, m_ReferenceAheadAxis.y, 0,
            m_ReferenceRightAxis.z, m_ReferenceUpAxis.z, m_ReferenceAheadAxis.z, 0,
                                 0,                   0,                      0, 1
        };
    }

    void FPSCamera::setPosition(const float3& pos)
    {
        position.x = pos.x;
        position.y = pos.y;
        position.z = pos.z;
    }

    void FPSCamera::setRotation(float Yaw, float Pitch)
    {
        m_fYawAngle = Yaw;
        m_fPitchAngle = Pitch;
    }

    void FPSCamera::setLookAt(const float3& LookAt)
    {
        float3 ViewDir = LookAt - position;
        ViewDir = ViewDir * getReferenceRotation();

        m_fYawAngle = atan2f(ViewDir.x, ViewDir.z);

        float fXZLen = sqrtf(ViewDir.z * ViewDir.z + ViewDir.x * ViewDir.x);
        m_fPitchAngle = -atan2f(ViewDir.y, fXZLen);
    }

    void FPSCamera::setProjAttribs(Float32           NearClipPlane,
                                   Float32           FarClipPlane,
                                   Float32           AspectRatio,
                                   Float32           FOV,
                                   SURFACE_TRANSFORM SrfPreTransform,
                                   bool              IsGL)
    {
        m_ProjAttribs.NearClipPlane = NearClipPlane;
        m_ProjAttribs.FarClipPlane = FarClipPlane;
        m_ProjAttribs.AspectRatio = AspectRatio;
        m_ProjAttribs.FOV = FOV;
        m_ProjAttribs.PreTransform = SrfPreTransform;
        m_ProjAttribs.IsGL = IsGL;

        float XScale = 0.f, YScale = 0.f;
        if (SrfPreTransform == SURFACE_TRANSFORM_ROTATE_90 ||
            SrfPreTransform == SURFACE_TRANSFORM_ROTATE_270 ||
            SrfPreTransform == SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90 ||
            SrfPreTransform == SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270)
        {
            // When the screen is rotated, vertical FOV becomes horizontal FOV
            XScale = 1.f / std::tan(FOV / 2.f);

            // Aspect ratio is width/height accounting for pretransform
            YScale = XScale / AspectRatio;
        }
        else
        {
            YScale = 1.f / std::tan(FOV / 2.f);
            XScale = YScale / AspectRatio;
        }

        float4x4 Proj;
        Proj._11 = XScale;
        Proj._22 = YScale;
        Proj.SetNearFarClipPlanes(NearClipPlane, FarClipPlane, IsGL);

        m_ProjMatrix = float4x4::Projection(m_ProjAttribs.FOV, m_ProjAttribs.AspectRatio, m_ProjAttribs.NearClipPlane, m_ProjAttribs.FarClipPlane, IsGL);
    }
}