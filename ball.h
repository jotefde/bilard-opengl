#pragma once
#include "mesh.h"

class Ball
{
public:
	//movement
	glm::vec2 position;
	glm::vec2 acceleration = glm::vec2(0);
	glm::vec2 speed = glm::vec2(0);
	glm::vec4 color = glm::vec4(1.0f);
	static const glm::vec4 colors[15];
	static float radius;
	static int iterator;

	float angle = 0;
	float dir = 0;
	int id;

	static Mesh* GetMesh();
	static ShaderProgram* GetShader();

	Ball(float, float);
	Ball(glm::vec2);
	glm::mat4 Render(glm::mat4, glm::mat4, glm::mat4);

	std::vector<int> collisionArray;

	void move(float x, float y);
	void update();
	bool collides(Ball* colBall);
	void bounce(Ball* colBall);
private:
	static Mesh* _mesh;
	static ShaderProgram* _shader;
};

