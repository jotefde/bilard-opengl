#include "ball.h"
#include "table.h"

Mesh* Ball::_mesh = nullptr;
ShaderProgram* Ball::_shader = nullptr;
float Ball::radius = 0.5f;
int Ball::iterator = 0;

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

Ball::Ball(float x = 0, float y = 0)
{
	position = glm::vec2(x, y);
	this->id = Ball::iterator++;
	for (int i = 0; i < 16; ++i) {
		this->collisionArray.push_back(false);
	}
}

Ball::Ball(glm::vec2 pos)
{
	position = pos;
	this->id = Ball::iterator++;
	for (int i = 0; i < 16; ++i) {
		this->collisionArray.push_back(false);
	}
}

glm::mat4 Ball::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(this->position, 0));
	M = glm::scale(M, glm::vec3(Ball::radius));
	M = glm::rotate(M, this->angle, glm::vec3(cosf(this->dir - PI / 2), sinf(this->dir - PI / 2), 0));
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

void Ball::move(float x, float y)
{
	this->acceleration += glm::vec2(x,y);
}

void Ball::update()
{
	this->speed += this->acceleration;

	this->position += this->speed;
	
	if (this->position.x + Ball::radius > Table::bounds.x || this->position.x - Ball::radius < -(Table::bounds.x)) {
		this->position.x = (this->position.x > 0) ? (Table::bounds.x - Ball::radius) : ( - (Table::bounds.x) + Ball::radius);
		this->speed.x *= -0.7f;
	}
	if (this->position.y + Ball::radius > Table::bounds.y || this->position.y - Ball::radius < -(Table::bounds.y)) {
		this->position.y = (this->position.y > 0) ? (Table::bounds.y - Ball::radius) : (- (Table::bounds.y) + Ball::radius);
		this->speed.y *= -0.7f;
	}
	//angle of rotation
	this->angle -= glm::length(this->speed);
	
	//rotation
	if (this->speed.x == 0 && this->speed.y == 0) this->dir = 0;
	else if (this->speed.x == 0) {
		if (this->speed.y > 0)
			this->dir = PI / 2;
		else
			this->dir = -PI / 2;
	}
	else if (this->speed.x < 0) {
		if (this->speed.y < 0) this->dir = atan(this->speed.y / this->speed.x) - PI;
		else this->dir = atan(this->speed.y / this->speed.x) + PI;
	}
	else this->dir = atan(this->speed.y / this->speed.x);

	//friction
	this->speed *= Table::frictionCoeff;

	this->acceleration = glm::vec2(0);

	//to not compute too much
	if (glm::length(this->speed) < 0.0001)
		this->speed = glm::vec2(0);
}

bool Ball::collides(Ball* colBall)
{
	if (glm::distance(this->position, colBall->position) <= Ball::radius * 2) return true;

	return false;
}

void Ball::bounce(Ball* colBall)
{
	if (!(this->collisionArray[colBall->id]) && !(colBall->collisionArray[this->id])) {

		this->collisionArray[colBall->id] = true;
		colBall->collisionArray[this->id] = true;

		float d = pow(glm::length(this->position - colBall->position), 2);
		glm::vec2 u_a = this->speed - (glm::dot(this->speed - colBall->speed, this->position - colBall->position) / d * (this->position - colBall->position));
		glm::vec2 u_b = colBall->speed - (glm::dot(colBall->speed - this->speed, colBall->position - this->position) / d * (colBall->position - this->position));
		this->speed = u_a;
		this->position += this->speed * 0.05f;
		colBall->speed = u_b;
		colBall->position += colBall->speed * 0.05f;

	}
}
