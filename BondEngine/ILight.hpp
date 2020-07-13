// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <corecrt_memory.h>

#include <..\DiligentCore\Common\interface\BasicMath.hpp>

namespace be::utils::render
{
    class ILight
    {
    public:
        ILight()
        {
            memset(this, 0, sizeof(ILight));
        }

        virtual ~ILight() = default;

        ILight(const ILight&) = default;
        ILight(ILight&&) = default;

        ILight operator=(const ILight& light)
        {
            this->direction = light.direction;
            this->ambient   = light.ambient;
            this->diffuse   = light.diffuse;

            return *this;
        }

    public:
        Diligent::Math::float4 direction;
        Diligent::Math::float4 ambient;
        Diligent::Math::float4 diffuse;
    };

    struct DirectionalLight : public ILight
    {
    public:
        DirectionalLight()
        {
            memset(this, 0, sizeof(DirectionalLight));
        }

        DirectionalLight operator=(const DirectionalLight& light)
        {
            this->direction = light.direction;
            this->ambient = light.ambient;
            this->diffuse = light.diffuse;

            return *this;
        }
    };

    struct PointLight : public ILight
    {
        PointLight()
        {
            memset(this, 0, sizeof(PointLight));
        }

        PointLight operator=(const PointLight& light)
        {
            this->intesivity = light.intesivity;
            this->direction = light.direction;
            this->position = light.position;
            this->ambient = light.ambient;
            this->diffuse = light.diffuse;

            return *this;
        }

    public:
        Diligent::Math::float4 position;
        Diligent::Math::float4 intesivity;
    };
}