#include "table.h"

Mesh* Table::_mesh = nullptr;
ShaderProgram* Table::_shader = nullptr;
float Table::frictionCoeff = 0.0f;
float Table::hole_bound = 0.7f;
float Table::bounce_factor = -0.7f;
glm::vec2 Table::bounds = glm::vec2(6.3f, 11.75f);
GLuint Table::field_texture = 0;
GLuint Table::table_texture = 0;

Mesh* Table::GetMesh()
{
	if (!_mesh) {
		_mesh = new Mesh("table.obj");
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

Table::Table(float fcoeff)
{
	Table::frictionCoeff = fcoeff;
}

glm::mat4 Table::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(0, 0, -0.2f));
	ShaderProgram* sp = Table::GetShader();
	Mesh* mesh = Table::GetMesh();
	sp->use();

	glUniform4fv(sp->u("color"), 1, glm::value_ptr(this->color));
	glUniform4f(sp->u("lightDir"), 1.0f, 1.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, mesh->vertices.data());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, mesh->normals.data());

	glEnableVertexAttribArray(sp->a("texCoord"));
	glm::vec2* texCoords0 = mesh->texCoords[0].data();
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Table::field_texture);
	glUniform1i(sp->u("fieldTexture"), 0);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));

	//this->DrawBounds();
	return M;
}

void Table::DrawBounds()
{
	glBegin(GL_LINES);

	float x = Table::bounds.x,
		y = Table::bounds.y;


	// left top conrner (-x, y)
	glVertex3f(-x, y - Table::hole_bound, 0.0f);
	glVertex3f(-x - Table::hole_bound, y, 0.0f);

	glVertex3f(-x - Table::hole_bound, y, 0.0f);
	glVertex3f(-x, y + Table::hole_bound, 0.0f);

	glVertex3f(-x, y + Table::hole_bound, 0.0f);
	glVertex3f(-x + Table::hole_bound, y, 0.0f);

	// top border (*, y)
	glVertex3f(-x + Table::hole_bound, y, 0.0f);
	glVertex3f(x - Table::hole_bound, y, 0.0f);

	// top right corner (x, y)
	glVertex3f(x - Table::hole_bound, y, 0.0f);
	glVertex3f(x, y + Table::hole_bound, 0.0f);

	glVertex3f(x, y + Table::hole_bound, 0.0f);
	glVertex3f(x + Table::hole_bound, y, 0.0f);

	glVertex3f(x + Table::hole_bound, y, 0.0f);
	glVertex3f(x, y - Table::hole_bound, 0.0f);

	// right top border (x, y)
	glVertex3f(x, y - Table::hole_bound, 0.0f);
	glVertex3f(x, Table::hole_bound, 0.0f);

	// center right corner (x, 0)
	glVertex3f(x, Table::hole_bound, 0.0f);
	glVertex3f(x + Table::hole_bound, Table::hole_bound/2, 0.0f);

	glVertex3f(x + Table::hole_bound, Table::hole_bound / 2, 0.0f);
	glVertex3f(x + Table::hole_bound, -Table::hole_bound / 2, 0.0f);

	glVertex3f(x + Table::hole_bound, -Table::hole_bound / 2, 0.0f);
	glVertex3f(x, -Table::hole_bound, 0.0f);

	// right bottom border (x, -y)
	glVertex3f(x, -Table::hole_bound, 0.0f);
	glVertex3f(x, -y + Table::hole_bound, 0.0f);

	// bottom right corner (x, -y)
	glVertex3f(x, -y + Table::hole_bound, 0.0f);
	glVertex3f(x + Table::hole_bound, -y, 0.0f);

	glVertex3f(x + Table::hole_bound, -y, 0.0f);
	glVertex3f(x, -y - Table::hole_bound, 0.0f);

	glVertex3f(x, -y - Table::hole_bound, 0.0f);
	glVertex3f(x - Table::hole_bound, -y, 0.0f);

	// bottom border (*, -y)
	glVertex3f(x - Table::hole_bound, -y, 0.0f);
	glVertex3f(-x + Table::hole_bound, -y, 0.0f);

	//bottom left corner (-x, -y)
	glVertex3f(-x + Table::hole_bound, -y, 0.0f);
	glVertex3f(-x, -y - Table::hole_bound, 0.0f);

	glVertex3f(-x, -y - Table::hole_bound, 0.0f);
	glVertex3f(-x - Table::hole_bound, -y, 0.0f);

	glVertex3f(-x - Table::hole_bound, -y, 0.0f);
	glVertex3f(-x, -y + Table::hole_bound, 0.0f);

	//left bottom border (-x, 0)
	glVertex3f(-x, -y + Table::hole_bound, 0.0f);
	glVertex3f(-x, -Table::hole_bound, 0.0f);

	// center left corner (-x, 0)
	glVertex3f(-x, -Table::hole_bound, 0.0f);
	glVertex3f(-x -Table::hole_bound, -Table::hole_bound/2, 0.0f);

	glVertex3f(-x -Table::hole_bound, -Table::hole_bound/2, 0.0f);
	glVertex3f(-x -Table::hole_bound, Table::hole_bound/2, 0.0f);

	glVertex3f(-x -Table::hole_bound, Table::hole_bound/2, 0.0f);
	glVertex3f(-x, Table::hole_bound, 0.0f);

	//left top border (-x, *)
	glVertex3f(-x, Table::hole_bound, 0.0f);
	glVertex3f(-x, y -Table::hole_bound, 0.0f);


	//glVertex3f(balls[0]->position.x + cosf(cue_angle) * 3.0f, balls[0]->position.y + sinf(cue_angle) * 3.0f, 0.0f);

	glEnd();
	glFlush();
}
