#include "ball.h"

Mesh* Ball::_mesh = nullptr;
ShaderProgram* Ball::_shader = nullptr;
const glm::vec4 Ball::colors[15] = {
		glm::vec4(1.0f, 1.0f, 0, 1),
		glm::vec4(0.0f, 0.0f, 1.0f, 1),
		glm::vec4(0.85f, 0.45f, 0.1f, 1),
		glm::vec4(0.3f, 0.0f, 0.55f, 1),
		glm::vec4(0.0f, 0.0f, 0.0f, 1),
		glm::vec4(0.88f, 0.65f, 0.4f, 1),
		glm::vec4(0.1f, 0.45f, 0.0f, 1),
		glm::vec4(0.7f, 0.0f, 0.05f, 1),
		glm::vec4(1.0f, 1.0f, 0, 1),
		glm::vec4(0.0f, 0.0f, 1.0f, 1),
		glm::vec4(0.85f, 0.45f, 0.1f, 1),
		glm::vec4(0.3f, 0.0f, 0.55f, 1),
		glm::vec4(0.88f, 0.65f, 0.4f, 1),
		glm::vec4(0.1f, 0.45f, 0.0f, 1),
		glm::vec4(0.7f, 0.0f, 0.05f, 1),
};

Mesh* Ball::GetMesh()
{
	if (!_mesh) {
		_mesh = new Mesh("ball.fbx");
	}
	return _mesh;
}

ShaderProgram* Ball::GetShader()
{
	if (!_shader) {
		_shader = new ShaderProgram("v_ball.glsl", NULL,  "f_ball.glsl");
	}
	return _shader;
}

Ball::Ball(float x = 0, float y = 0, float z = 0)
{
	position = Position(x, y, z);
}

Ball::Ball(Position pos)
{
	position = pos;
}

glm::mat4 Ball::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(this->position.X, this->position.Y, this->position.Z));
	M = glm::scale(M, glm::vec3(0.5f));

	ShaderProgram* sp = Ball::GetShader();
	Mesh* mesh = Ball::GetMesh();
	sp->use();

	glUniform4fv(sp->u("color"), 1, glm::value_ptr(this->color));
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
