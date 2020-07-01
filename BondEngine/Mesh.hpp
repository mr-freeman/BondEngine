// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <..\DiligentCore\Common\interface\BasicMath.hpp>

#include <filesystem>
#include <vector>
#include <string>

#include <VertexShader.hpp>
#include <PixelShader.hpp>

#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>

#include <Texture.hpp>

#include <File.hpp>
#include <IGameObject.hpp>

#include <CodallaMesh.hpp>

namespace be
{
    class Mesh : public File
    {
    public:
        Mesh(const std::string& file_name, const std::filesystem::path& file_extension)
        {
            name = file_name;
            directory = FileSystem::getGameDataDirectory("meshes");
            extension = file_extension.string();
            full_name = this->directory / (this->name + this->extension);

            check(false);
        }

        virtual ~Mesh() = default;

    public:
        std::filesystem::path getFullName() const { return full_name; }

        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;

        Mesh operator=(const Mesh&) = delete;
        Mesh operator=(Mesh&&) = delete;

    private:
        std::unique_ptr<render::utils::VertexBuffer> vertex_buffer;
        std::unique_ptr<render::utils::IndexBuffer> inxed_buffer;

        std::unique_ptr<render::utils::VertexShader> vertex_shader;
        std::unique_ptr<render::utils::PixelShader> pixel_shader;

        std::vector<std::unique_ptr<UnifiedTextureFile>> textures;
    };

    using be::utils::BaseMesh;

    class CodallaMesh : public Mesh
    {

    public:
        void au()
        {
            cm.load("ds");

        }

    private:
        be::utils::CodallaMesh cm;
    };
}