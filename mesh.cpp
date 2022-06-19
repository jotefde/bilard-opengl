#include "mesh.h"

Mesh::Mesh(std::string model_file)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	std::cout << importer.GetErrorString() << std::endl;

	aiMesh* mesh = scene->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		this->vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		this->normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));


		/*aiVector3D texCoord = mesh->mTextureCoords[0][i];
		this->texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));*/
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {

		aiFace& face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++) {
			this->indices.push_back(face.mIndices[j]);
		}
	}
}