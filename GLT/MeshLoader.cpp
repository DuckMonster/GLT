#include "MeshLoader.hpp"

using namespace std;
using namespace glt;
using namespace glm;

void MeshData::append(MeshData& other) {
	positions.insert(positions.end(), other.positions.begin(), other.positions.end());
	normals.insert(normals.end(), other.normals.begin(), other.normals.end());
	uvs.insert(uvs.end(), other.uvs.begin(), other.uvs.end());
}

FbxManager* MeshLoader::manager = nullptr;

vector<float> MeshLoader::readControlPoints(FbxMesh* mesh) {
	vector<float> pointList;

	//Loop through triangles
	int triCount = mesh->GetPolygonCount();
	for (int i = 0; i < triCount; i++) {

		//Loop through vertices
		for (int j = 0; j < 3; j++) {
			//Get the control point position
			int ctrlPointIndex = mesh->GetPolygonVertex(i, j);
			FbxVector4 point = mesh->GetControlPointAt(ctrlPointIndex);

			//Add it to the list
			pointList.push_back(static_cast<float>(point.mData[0]));
			pointList.push_back(static_cast<float>(point.mData[1]));
			pointList.push_back(static_cast<float>(point.mData[2]));
		}
	}

	return pointList;
}
vector<float> MeshLoader::readNormals(FbxMesh* mesh) {
	//No normals on mesh
	if (mesh->GetElementNormalCount() < 1) {
		cout << "Invalid normal data...\n";
		return vector<float>();
	}

	//Return list
	vector<float> normalList;

	//Normal data
	FbxGeometryElementNormal* normalData = mesh->GetElementNormal(0);

	//Error checking
	if ((normalData->GetMappingMode() != FbxGeometryElement::eByControlPoint &&
		normalData->GetMappingMode() != FbxGeometryElement::eByPolygonVertex) ||
		(normalData->GetReferenceMode() != FbxGeometryElement::eDirect &&
			normalData->GetReferenceMode() != FbxGeometryElement::eIndexToDirect)) {
		cout << "Invalid normal data ...\n";
		return vector<float>();
	}

	//Loop through each triangle
	int triCount = mesh->GetPolygonCount();
	for (int i = 0; i < triCount; i++) {

		//... and each vertex of each triangle
		for (int j = 0; j < 3; j++) {
			//Is direct referenced?
			//Direct means the Normal array points directly to the data
			//Otherwise you have to find the Normal data index in a separate list
			bool direct = normalData->GetReferenceMode() == FbxGeometryElement::eDirect;

			//Mapping index (Control point or Polygon Vertex)
			int mappingIndex = -1;

			//Control point
			if (normalData->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				mappingIndex = mesh->GetPolygonVertex(i, j);

			//Polygon vertex
			else if (normalData->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				mappingIndex = i * 3 + j; //FaceN * 3 + Vertex

										  //Index in direct array
			int index = direct ? mappingIndex : normalData->GetIndexArray().GetAt(mappingIndex);

			//Get the data
			FbxVector4 vNormal = normalData->GetDirectArray().GetAt(index);

			//Add to list
			normalList.push_back(static_cast<float>(vNormal.mData[0]));
			normalList.push_back(static_cast<float>(vNormal.mData[1]));
			normalList.push_back(static_cast<float>(vNormal.mData[2]));
		}
	}

	return normalList;
}
vector<float> MeshLoader::readUV(FbxMesh* mesh) {
	//No normals on mesh
	if (mesh->GetElementNormalCount() < 1) {
		cout << "Invalid normal data...\n";
		return vector<float>();
	}

	//Return list
	vector<float> uvList;

	//UV data
	FbxGeometryElementUV* uvData = mesh->GetElementUV(0);

	FbxGeometryElement::EReferenceMode ref = uvData->GetReferenceMode();
	FbxGeometryElement::EMappingMode map = uvData->GetMappingMode();

	//Error checking
	if ((uvData->GetMappingMode() != FbxGeometryElement::eByControlPoint &&
		uvData->GetMappingMode() != FbxGeometryElement::eByPolygonVertex) ||
		(uvData->GetReferenceMode() != FbxGeometryElement::eDirect &&
			uvData->GetReferenceMode() != FbxGeometryElement::eIndexToDirect)) {
		cout << "Invalid UV data ...\n";
		return vector<float>();
	}

	//Loop through each triangle
	int triCount = mesh->GetPolygonCount();
	for (int i = 0; i < triCount; i++) {

		//... and each vertex of each triangle
		for (int j = 0; j < 3; j++) {
			//Is direct referenced?
			//Direct means the Normal array points directly to the data
			//Otherwise you have to find the Normal data index in a separate list
			bool direct = uvData->GetReferenceMode() == FbxGeometryElement::eDirect;

			//Mapping index (Control point or Polygon Vertex)
			int mappingIndex = -1;

			//Control point
			if (uvData->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				mappingIndex = mesh->GetPolygonVertex(i, j);

			//Polygon vertex
			else if (uvData->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				mappingIndex = i * 3 + j; //FaceN * 3 + Vertex

										  //Index in direct array
			int index = direct ? mappingIndex : uvData->GetIndexArray().GetAt(mappingIndex);

			//Get the data
			FbxVector2 vUV = uvData->GetDirectArray().GetAt(index);

			//Add to list
			uvList.push_back(static_cast<float>(vUV.mData[0]));
			uvList.push_back(static_cast<float>(vUV.mData[1]));
		}
	}

	return uvList;
}

MeshData MeshLoader::processMesh(FbxMesh* mesh) {
	MeshData data;

	vector<float> vPoints = readControlPoints(mesh);
	vector<float> vNormals = readNormals(mesh);
	vector<float> vUVs = readUV(mesh);

	data.positions.assign(vPoints.begin(), vPoints.end());
	data.normals.assign(vNormals.begin(), vNormals.end());
	data.uvs.assign(vUVs.begin(), vUVs.end());

	return std::move(data);
}

vector<MeshData> MeshLoader::processNode(FbxNode* root) {
	vector<MeshData> data;

	int childCount = root->GetChildCount();

	for (int i = 0; i < childCount; i++) {
		FbxNode* child = root->GetChild(i);

		//Is this child a mesh?
		FbxMesh* mesh = child->GetMesh();
		if (mesh) {
			//Then add this mesh to the data
			data.push_back(processMesh(mesh));
		}

		//Recursively add this childs' childrens meshes
		vector<MeshData> childData = processNode(child);

		//Append childs meshes to data list
		data.insert(data.end(), childData.begin(), childData.end());
	}

	return data;
}

Mesh* MeshLoader::loadFBX(const char* fileName) {
	vector<MeshData> data = loadFBXRaw(fileName);

	if (data.size() > 0) {
		//Combine data
		MeshData combinedData;

		for (MeshData d : data)
			combinedData.append(d);

		Mesh* mesh = new Mesh();
		mesh->setVertices(&combinedData.positions[0], combinedData.positions.size() * sizeof(float));
		mesh->setUVS(&combinedData.uvs[0], combinedData.uvs.size() * sizeof(float));
		mesh->setNormals(&combinedData.normals[0], combinedData.normals.size() * sizeof(float));

		return mesh;
	}
	else
		return nullptr;
}

vector<MeshData> MeshLoader::loadFBXRaw(const char* fileName) {
	//Create manager singleton
	if (manager == nullptr) {
		manager = FbxManager::Create();
		FbxIOSettings* settings = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(settings);
	}

	//Importer and scene
	FbxImporter* importer = FbxImporter::Create(manager, "");
	FbxScene* scene = FbxScene::Create(manager, "");

	//Initialize and load the FBX
	//Exit if there is an error
	if (!importer->Initialize(fileName, -1, manager->GetIOSettings())) {
		cout << "Failed to open FBX!\n";
		cout << importer->GetStatus().GetErrorString() << "\n";
		return vector<MeshData>();
	}

	if (!importer->Import(scene)) {
		cout << "Failed to import scene!\n";
		cout << importer->GetStatus().GetErrorString() << "\n";
		return vector<MeshData>();
	}

	//Cleanup importer
	importer->Destroy();

	//Create vector
	FbxNode* root = scene->GetRootNode();
	vector<MeshData> resultList = processNode(root);

	scene->Destroy(true);
	return resultList;
}

//vector<vector<float>> MeshLoader::loadFBXRaw(const char* fileName, bool triangulate) {
//	//Create manager singleton
//	if (manager == nullptr) {
//		manager = FbxManager::Create();
//		FbxIOSettings* settings = FbxIOSettings::Create(manager, IOSROOT);
//		manager->SetIOSettings(settings);
//	}
//
//	//Create vector
//	vector<vector<float>> resultList;
//
//	//Importer and scene
//	FbxImporter* importer = FbxImporter::Create(manager, "");
//	FbxScene* scene = FbxScene::Create(manager, "");
//
//	//Initialize and load the FBX
//	//Exit if there is an error
//	if (!importer->Initialize(fileName, -1, manager->GetIOSettings()))
//		return resultList;
//
//	if (!importer->Import(scene))
//		return resultList;
//
//	//Cleanup importer
//	importer->Destroy();
//
//	//Root node
//	FbxNode* root = scene->GetRootNode();
//	if (root) {
//		//Go though child nodes
//		for (int i = 0; i < root->GetChildCount(); i++) {
//			FbxNode* child = root->GetChild(i);
//
//			//If it isn't a mesh, move on
//			if (child->GetNodeAttribute() == NULL ||
//				child->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
//				continue;
//
//			FbxMesh* mesh = (FbxMesh*)child->GetNodeAttribute();
//			FbxVector4* vertices = mesh->GetControlPoints();
//
//			vector<float> meshData;
//
//			//Make all the faces
//			for (int j = 0; j < mesh->GetPolygonCount(); j++) {
//				int vertCount = mesh->GetPolygonSize(j);
//
//				//Load face
//				if (triangulate && vertCount > 3) {
//					for(int k = 1; k < vertCount-1; k++) {
//						vector<int> polygonVertexIndicies;
//						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, 0));
//
//						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, k));
//						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, k+1));
//
//						//addFace(&meshData, vertices, &polygonVertexIndicies[0], 3);
//					}
//				}
//				else {
//					vector<int> polygonVertexIndicies;
//					for (int k = 0; k < vertCount; k++) {
//						polygonVertexIndicies.push_back(mesh->GetPolygonVertex(j, k));
//					}
//
//					//(&meshData, vertices, &polygonVertexIndicies[0], vertCount);
//				}
//			}
//
//			resultList.push_back(meshData);
//		}
//	}
//
//	scene->Destroy(true);
//	return resultList;
//}