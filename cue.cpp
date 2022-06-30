#include "cue.h"
#include "ball.h"

Mesh* Cue::_mesh = nullptr;
ShaderProgram* Cue::_shader = nullptr;

Mesh* Cue::GetMesh()
{
	if (!_mesh) {
		_mesh = new Mesh("cue.obj");
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
	cue_angle = 0.0f;
}

Cue::Cue(glm::vec2 pos)
{
	position = pos;
	cue_angle = 0.0f;
}

glm::mat4 Cue::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(this->position, 0.0f));
	M = glm::rotate(M, cue_angle + PI/2, glm::vec3(0, 0, 1.0f));
	M = glm::rotate(M, PI / 10, glm::vec3(1.0f, 0, 0));

	ShaderProgram* sp = Cue::GetShader();
	Mesh* mesh = Cue::GetMesh();

	sp->use();

	glUniform4f(sp->u("color"), 0.0f, 0.5f, 1.0f, 1.0f);
	glUniform4f(sp->u("lightDir"), 1.0f, 1.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, mesh->vertices.data());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, mesh->normals.data());

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, mesh->texCoords.data());

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));

	return M;
}