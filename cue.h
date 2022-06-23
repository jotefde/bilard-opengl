#pragma once
#include "mesh.h"

class Cue
{
public:
	//movement
	glm::vec2 position;
	float cue_angle;

	static Mesh* GetMesh();
	static ShaderProgram* GetShader();
	Cue(float, float);
	Cue(glm::vec2);
	glm::mat4 Render(glm::mat4, glm::mat4, glm::mat4);
	void move(float x, float y);
	void update();
private:
	static Mesh* _mesh;
	static ShaderProgram* _shader;
};

