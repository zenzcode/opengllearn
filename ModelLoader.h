#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glew.h>
#include "Mesh.h"
#include "Texture.h"
#include "CommonValues.h"

class ModelLoader
{
public:
	ModelLoader();
	void LoadModel(const std::string filePath);
	void RenderModel();
	void ClearModel();
	~ModelLoader();
private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterial(const aiScene* scene);

	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures;
	std::vector<unsigned int> meshToMat;
};

class Model
{
};

