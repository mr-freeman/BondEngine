// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <CodallaLoader.hpp>

#include <../third-party/tinyxml2/tinyxml2.h>

#include <Log.hpp>

namespace be::utils
{
	CodallaLoader::CodallaLoader(const fs::path& file = "")
	{
		if (!file.empty())
			load(file);
	}

	void CodallaLoader::load(const fs::path& file)
	{
		using namespace math;

		tinyxml2::XMLDocument xml;
		xml.LoadFile(file.string().c_str());
		auto* COLLADA = xml.FirstChildElement("COLLADA");
		std::cout << COLLADA->FirstAttribute()->Value() << std::endl;

		auto* library_geometries = COLLADA->FirstChildElement("library_geometries");
		vector<tinyxml2::XMLElement*> geometries;

		for (auto* current_geometry = library_geometries->FirstChildElement("geometry");
			 current_geometry != nullptr;
			 current_geometry = current_geometry->NextSiblingElement())
		{
			geometries.push_back(current_geometry);
		}

		count = geometries.size();
		for (auto* item : geometries)
		{
			auto* mesh = item->FirstChildElement("mesh");
			auto* sources = mesh->FirstChildElement("source");

			auto* vertices = sources;
			sources = sources->NextSiblingElement();

			auto* normals = sources;
			sources = sources->NextSiblingElement();

			auto* texCoords = sources;
			auto* faces = mesh->FirstChildElement("polygons");

			tinyxml2::XMLElement* indices = nullptr;

			if (faces != nullptr)
				indices = faces->FirstChildElement("p");

			BaseMesh newMesh;
			// Vertices
			auto* vertex_array = vertices->FirstChildElement("float_array")->GetText();
			std::stringstream ss(vertex_array);
			while (!ss.eof())
			{
				float3 newVert;
				ss >> newVert.x >> newVert.y >> newVert.z;
				newMesh.vertices.push_back(newVert);
			}
			//-----------------

			// Normals
			auto* normal_array = normals->FirstChildElement("float_array")->GetText();
			ss = std::stringstream(normal_array);
			while (!ss.eof())
			{
				float3 newNorm;
				ss >> newNorm.x >> newNorm.y >> newNorm.z;
				newMesh.normals.push_back(newNorm);
			}
			//---------------------

			// Texcoords
			auto* texcoord_array = texCoords->FirstChildElement("float_array")->GetText();
			ss = std::stringstream(texcoord_array);
			while (!ss.eof())
			{
				float2 newTexcoord;
				ss >> newTexcoord.x >> newTexcoord.y;
				newTexcoord.y = 1 - newTexcoord.y;
				newMesh.texcoords.push_back(newTexcoord);
			}
			//---------------------

			// Indices
			ss = std::stringstream(indices->GetText());
			while (!ss.eof())
			{
				int3 newIndex;
				ss >> newIndex.x >> newIndex.y >> newIndex.z;
				utils::log::debug("{}, {}, {}", newIndex.x, newIndex.y, newIndex.z);
				newMesh.indices.push_back(newIndex);
			}
			//----------------------

			temp_meshes.push_back(newMesh);
		}

		for (auto mesh_item : temp_meshes)
		{
			// int index = 0;
			BaseMesh result_mesh;
			for (auto index_item : mesh_item.indices)
			{
				float3 vert = mesh_item.vertices[index_item.x];
				float3 norm = mesh_item.normals[index_item.y];
				float2 texc = mesh_item.texcoords[index_item.z];

				result_mesh.vertices.push_back(vert);
				result_mesh.normals.push_back(norm);
				result_mesh.texcoords.push_back(texc);
				result_mesh.indices_count.push_back(count++);
			}

			submeshes.push_back(result_mesh);
		}
	}
}