#include "cue.h"

Mesh* Cue::_mesh = nullptr;
ShaderProgram* Cue::_shader = nullptr;

Mesh* Cue::GetMesh()
{
	if (!_mesh) {
		_mesh = new Mesh("ball.fbx");
	}
	return _mesh;
}

ShaderProgram* Cue::GetShader()
{
	if (!_shader) {
		_shader = new ShaderProgram("v_cue.glsl", NULL,  "f_cue.glsl");
	}
	return _shader;
}

Cue::Cue(float x = 0, float y = 0)
{
	position = glm::vec2(x, y);
}

Cue::Cue(glm::vec2 pos)
{
	position = pos;
}

glm::mat4 Cue::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(this->position, 0));
	M = glm::scale(M, glm::vec3(0.5f));

	ShaderProgram* sp = Cue::GetShader();
	//Mesh* mesh = Cue::GetMesh();
	sp->use();

	glUniform4fv(sp->u("color"), 1, glm::value_ptr(this->color));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glBegin(GL_LINES);

	glVertex3f(0, 0, 0.0f);
	glVertex3f(12.0f, 0, 0.0f);
	//glVertex3f(balls[0]->position.x + cosf(cue_angle) * 3.0f, balls[0]->position.y + sinf(cue_angle) * 3.0f, 0.0f);

	glEnd();
	glFlush();

	return M;
}

void Cue::move(float x, float y)
{
	this->acceleration += glm::vec2(x,y);
}

void Cue::update()
{
	this->speed += this->acceleration;

	this->position += this->speed;

	this->speed *= 0.995f;
	this->acceleration = glm::vec2(0);
	if (glm::length(this->speed) < 0.0001)
		this->speed = glm::vec2(0);
}
