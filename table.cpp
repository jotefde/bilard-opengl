#include "table.h"

Mesh* Table::_mesh = nullptr;
ShaderProgram* Table::_shader = nullptr;

Mesh* Table::GetMesh()
{
	if (!_mesh) {
		_mesh = new Mesh("stol.fbx");
	}
	return _mesh;
}

ShaderProgram* Table::GetShader()
{
	if (!_shader) {
		_shader = new ShaderProgram("v_table.glsl", NULL, "f_table.glsl");
	}
	return _shader;
}

Table::Table()
{
}

glm::mat4 Table::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(0, 0, -0.1f));
	ShaderProgram* sp = Table::GetShader();
	Mesh* mesh = Table::GetMesh();
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
