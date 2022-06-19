#pragma once
#include "mesh.h"

class Ball
{
public:
	Position position;
	glm::vec4 color = glm::vec4(1.0f);
	static Mesh* GetMesh();
	static ShaderProgram* GetShader();
	Ball(float, float, float);
	Ball(Position);
	glm::mat4 Render(glm::mat4, glm::mat4, glm::mat4);

	static const glm::vec4 colors[15];
private:
	static Mesh* _mesh;
	static ShaderProgram* _shader;
};

