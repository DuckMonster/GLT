#include "MeshLoader.hpp"

using namespace std;
using namespace glt;

FbxManager* MeshLoader::manager = nullptr;

vector<Mesh*> MeshLoader::loadFBX(const char* fileName, bool triangulate) {
	vector<vector<float>> rawData = loadFBXRaw(fileName, triangulate);

	vector<Mesh*> resultList(rawData.size());

	for (int i = 0; i < resultList.size(); i++) {
		resultList[i] = new Mesh();
		resultList[i]->setVertices(&rawData[i][0], rawData[i].size() * sizeof(float));
	}

	return resultList;
}

vector<vector<float>> MeshLoader::loadFBXRaw(const char* fileName, bool triangulate) {
	//Create manager singleton
	if (manager == nullptr) {
		manager = FbxManager::Create();
		FbxIOSettings* settings = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(settings);
	}

	//Create vector
	vector<vector<float>> resultList;

	//Importer and scene
	FbxImporter* importer = FbxImporter::Create(manager, "");
	FbxScene* scene = FbxScene::Create(manager, "");

	//Initialize and load the FBX
	//Exit if there is an error
	if (!importer->Initialize(fileName, -1, manager->GetIOSettings()))
		return resultList;

	if (!importer->Import(scene))
		return resultList;

	//Cleanup importer
	importer->Destroy();

	//Root node
	FbxNode* root = scene->GetRootNode();
	if (root) {
		//Go though child nodes
		for (int i = 0; i < root->GetChildCount(); i++) {
			FbxNode* child = root->GetChild(i);

			//If it isn't a mesh, move on
			if (child->GetNodeAttribute() == NULL ||
				child->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* mesh = (FbxMesh*)child->GetNodeAttribute();
			FbxVector4* vertices = mesh->GetControlPoints();

			vector<float> meshData;

			//Make all the faces
			for (int j = 0; j < mesh->GetPolygonCount(); j++) {
				int vertCount = mesh->GetPolygonSize(j);

				//Load face
				if (triangulate && vertCount > 3) {
					for(int k = 1; k < vertCount-1; k++) {
						vector<int> polygonVertexIndicies;
						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, 0));

						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, k));
						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, k+1));

						addFace(&meshData, vertices, &polygonVertexIndicies[0], 3);
					}
				}
				else {
					vector<int> polygonVertexIndicies;
					for (int k = 0; k < vertCount; k++)
						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, k));

					addFace(&meshData, vertices, &polygonVertexIndicies[0], vertCount);
				}
			}

			resultList.push_back(meshData);
		}
	}

	scene->Destroy(true);
	return resultList;
}

void MeshLoader::addFace(vector<float>* list, FbxVector4* vertices, int* indicies, size_t indexCount) {
	for (int i = 0; i < indexCount; i++) {
		list->push_back(vertices[indicies[i]].mData[0]);
		list->push_back(vertices[indicies[i]].mData[1]);
		list->push_back(vertices[indicies[i]].mData[2]);
	}
}