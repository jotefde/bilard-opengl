#pragma once
#include "mesh.h"

class Cue
{
public:
	//movement
	glm::vec2 position;
	glm::vec2 acceleration = glm::vec2(0);
	glm::vec2 speed = glm::vec2(0);
	glm::vec4 color = glm::vec4(1.0f);

	static Mesh* GetMesh();
	static ShaderProgram* GetShader();
	Cue(float, float);
	Cue(glm::vec2);
	glm::mat4 Render(glm::mat4, glm::mat4, glm::mat4);
	static const glm::vec4 colors[15];
	void move(float x, float y);
	void update();
private:
	static Mesh* _mesh;
	static ShaderProgram* _shader;
};

