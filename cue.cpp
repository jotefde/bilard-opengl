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
	M = glm::rotate(M, cue_angle + PI, glm::vec3(0, 0, 1.0f));

	ShaderProgram* sp = Cue::GetShader();
	//Mesh* mesh = Cue::GetMesh();
	sp->use();

	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glBegin(GL_LINES);

	glVertex3f(4.0f, 0, 0.0f);
	glVertex3f(15.0f, 0, 0.0f);
	//glVertex3f(balls[0]->position.x + cosf(cue_angle) * 3.0f, balls[0]->position.y + sinf(cue_angle) * 3.0f, 0.0f);

	glEnd();
	glFlush();

	return M;
}