// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <..\DiligentCore\Common\interface\BasicMath.hpp>

namespace be::utils
{
    struct BaseMesh
    {
        vector<math::float3> vertices;
        vector<math::float3> normals;
        vector<math::float2> texcoords;
        vector<math::int3> indices;

        vector<int> indices_count;
    };

    // Simple Codalla Mesh Loader.
    class CodallaLoader
    {
    public:
        CodallaLoader(const fs::path& file);
       ~CodallaLoader() = default;

        CodallaLoader(const CodallaLoader&) = default;
        CodallaLoader(CodallaLoader&&) = default;

        CodallaLoader operator=(const CodallaLoader&) = delete;
        CodallaLoader operator=(CodallaLoader&&) = delete;

    public:
        void load(const fs::path& file);

        const vector<BaseMesh>& getSubMeshes() const { return submeshes; }
        const int& getCount() const { return count; }

    private:
        vector<BaseMesh> temp_meshes; // for calculations.
        vector<BaseMesh> submeshes;

        int count;
    };
}