#pragma once
#include "Mesh.hpp"
#include <vector>
#include <fbxsdk.h>

namespace glt {
	struct MeshData {
	public:
		std::vector<float>		positions;
		std::vector<float>		normals;
		std::vector<float>		uvs;

		MeshData() {}

		void append(MeshData& other);
	};

	class MeshLoader {
	public:
		static Mesh*					loadFBX(const char* fileName);
		static std::vector<MeshData>	loadFBXRaw(const char* fileName);

	private:
		static FbxManager* manager;

		//Process a single FBX node. Returns if node was a mesh or not
		static std::vector<MeshData>	processNode(FbxNode* root);

		//Process a single Mesh. Returns vertex positions, normals and uvs
		static MeshData					processMesh(FbxMesh* mesh);

		//Read control points (positions) in polygon-vertex order
		static std::vector<float>		readControlPoints(FbxMesh* mesh);
		//Read normals
		static std::vector<float>		readNormals(FbxMesh* mesh);
		//Read UVs
		static std::vector<float>		readUV(FbxMesh* mesh);
	};
}