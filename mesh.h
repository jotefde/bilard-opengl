#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderprogram.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "constants.h"

class Mesh
{
public:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec4> colors;
	std::vector<unsigned int> indices;

	Mesh(std::string model_file);
};