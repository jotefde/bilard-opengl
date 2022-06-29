#pragma once

#include "mesh.h"
#include <string>

class Table
{
public:
	static Mesh* GetMesh();
	static ShaderProgram* GetShader();
	glm::vec4 color = glm::vec4(1.0f);
	static float frictionCoeff;
	static glm::vec2 bounds;
	static float hole_bound;
	static float bounce_factor;
	Table(float);
	glm::mat4 Render(glm::mat4 V, glm::mat4 P, glm::mat4 M);
	void DrawBounds();
private:
	static Mesh* _mesh;
	static ShaderProgram* _shader;
};

