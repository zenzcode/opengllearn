#include "ModelLoader.h"
ModelLoader::ModelLoader() {

}

void ModelLoader::LoadModel(const std::string filePath) {
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	if (!scene) {
		std::cerr << "Could not load Model." << std::endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);

	//TODO: Load Materials
	LoadMaterial(scene);
}

void ModelLoader::LoadNode(aiNode* node, const aiScene* scene) {
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		//node saves mesh storage in scene array, so we need to access it like this
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		LoadNode(node->mChildren[i], scene);
	} 
}

void ModelLoader::LoadMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	//recreate our vertices form
	//load vertices from mesh
	for (size_t i = 0; i < mesh->mNumVertices; ++i) {
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		//check if mesh has uv data
		if (mesh->HasTextureCoords(0)) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0, 0 });
		}

		//add normals
		//no need to check if normals exist, because we activated GenSmoothNormals
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
	}


	for (size_t i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* ourMesh = new Mesh();
	ourMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshes.push_back(ourMesh);
	meshToMat.push_back(mesh->mMaterialIndex);
}


void ModelLoader::LoadMaterial(const aiScene* scene) {
	textures.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; ++i) {
		aiMaterial* mat = scene->mMaterials[i];

		textures[i] = nullptr;
		std::cout << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
		if (mat->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;

			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				size_t startPos = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(startPos + 1);
				std::string pathToTexture = std::string("Textures/") + fileName;
				std::cout << pathToTexture;

				textures[i] = new Texture(pathToTexture.c_str());

				if (!textures[i]->LoadTexture()) {
					std::cerr << "Could not load texture at " + pathToTexture << std::endl;
					SAFE_DELETE(textures[i]);
					return;
				}
			}
		}

		if (!textures[i]) {
			textures[i] = new Texture("Images/purple.png");
			textures[i]->LoadTexture();
		}
	}
}

void ModelLoader::RenderModel() {
	for (size_t i = 0; i < meshes.size(); ++i) {
		int matIndex = meshToMat[i];

		if (matIndex < textures.size() && textures[matIndex]) {
			textures[matIndex]->UseTexture();
		}

		meshes[i]->RenderMesh();
	}
}

void ModelLoader::ClearModel() {
	for (size_t i = 0; i < meshes.size(); ++i) {
		SAFE_DELETE(meshes[i]);
	}

	for (size_t i = 0; i < textures.size(); ++i) {
		SAFE_DELETE(textures[i]);
	}
}

ModelLoader::~ModelLoader() {
	ClearModel();
}