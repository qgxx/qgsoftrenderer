#include "Model.hpp"

#include <map>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Texture.hpp"
#include "Pipeline.hpp"

namespace sr {

class AssimpImporterWrapper final{
public:
	//textureDict is for avoiding redundant loading
	std::map<std::string, int> textureDict = {};
	std::string directory = "";
	bool generatedMipmap = false;

	Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
		Mesh drawable;

		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			// positions
			vertex.m_vpositions = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			// normals
			if (mesh->HasNormals())
			{
				vertex.m_vnormals = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vertex.m_vtexcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				// tangent
				vertex.m_vtangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				// bitangent
				vertex.m_vbitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			}
			else
			{
				vertex.m_vtexcoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto loadFunc = [&](aiTextureType type) -> int
		{
			for (int i = 0; i < material->GetTextureCount(type); ++i)
			{
				aiString str;
				material->GetTexture(type, i, &str);
				if (textureDict.find(str.C_Str()) != textureDict.end())
				{
					//Already loaded
					return textureDict[str.C_Str()];
				}
				else
				{
					Texture::ptr diffTex = std::make_shared<Texture>(generatedMipmap);
					bool success = diffTex->loadTextureFromFile(directory + '/' + str.C_Str());
					auto texId = Pipeline::uploadTexture(diffTex);
					textureDict.insert({ str.C_Str(), texId });
					return texId;
				}
			}
			return -1;
		};

		//Texture maps
		drawable.setDiffuseMapTexId(loadFunc(aiTextureType_DIFFUSE));
		drawable.setSpecularMapTexId(loadFunc(aiTextureType_SPECULAR));
		drawable.setNormalMapTexId(loadFunc(aiTextureType_HEIGHT));
		drawable.setGlowMapTexId(loadFunc(aiTextureType_EMISSIVE));

		drawable.setVertices(vertices);
		drawable.setIndices(indices);

		return drawable;
	}

	void processNode(aiNode *node, const aiScene *scene, std::vector<Mesh> &drawables)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			drawables.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, drawables);
		}
	}
};


void Model::importMeshFromFile(const std::string &path, bool generatedMipmap) {
	for (auto &mesh : m_meshes)
	{
		mesh.clear();
	}
	m_meshes.clear();

	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals 
		| aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_SplitLargeMeshes | aiProcess_FixInfacingNormals);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		exit(1);
	}

	// retrieve the directory path of the filepath
	AssimpImporterWrapper wrapper;
	wrapper.generatedMipmap = generatedMipmap;
	wrapper.directory = path.substr(0, path.find_last_of('/'));
	wrapper.processNode(scene->mRootNode, scene, m_meshes);
	
}

void Model::clear()
{
	for (auto &mesh : m_meshes)
	{
		mesh.clear();
	}
}

Model::Model(const std::string &path, bool generatedMipmap)
{
	importMeshFromFile(path, generatedMipmap);
}

unsigned int Model::getDrawableMaxFaceNums() const
{
	unsigned int num = 0;
	for (const auto &mesh : m_meshes)
	{
		num = std::max(num, (unsigned int)mesh.getIndices().size() / 3);
	}
	return num;
}

} // namespace sr