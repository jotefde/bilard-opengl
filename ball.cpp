#include "ball.h"
#include "table.h"
#include <cmath>
#include <vector>

Mesh* Ball::_mesh = nullptr;
ShaderProgram* Ball::_shader = nullptr;
float Ball::radius = 0.5f;
int Ball::iterator = 0;
GLuint Ball::texture = 0;

Mesh* Ball::GetMesh()
{
	if (!_mesh) {
		_mesh = new Mesh("ball.obj");
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

Ball::Ball(glm::vec2 pos)
{
	this->position = pos;
	this->id = Ball::iterator++;
	for (int i = 0; i < 16; ++i) {
		this->collisionArray.push_back(false);
	}


	float texH = 324.0f / 5184.0f;
	std::vector<glm::vec2> _texCoords = Ball::GetMesh()->texCoords[0];
	for (auto& row : _texCoords)
	{
		row.y /= 15.0f;
		row.y += this->id * texH;
	}
	this->texCoords = _texCoords;
}

glm::mat4 Ball::Render(glm::mat4 V, glm::mat4 P, glm::mat4 M)
{
	M = glm::translate(M, glm::vec3(this->position, 0));
	M = glm::scale(M, glm::vec3(Ball::radius));
	M = glm::rotate(M, this->angle, glm::vec3(cosf(this->dir - PI / 2), sinf(this->dir - PI / 2), 0));
	ShaderProgram* sp = Ball::GetShader();
	Mesh* mesh = Ball::GetMesh();
	sp->use();
	glShadeModel(GL_SMOOTH);
	glUniform4fv(sp->u("color"), 1, glm::value_ptr(this->color));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, mesh->vertices.data());

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, mesh->normals.data());

	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, this->texCoords.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Ball::texture);
	glUniform1i(sp->u("tex"), 0);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));
	return M;
}

void Ball::move(float x, float y)
{
	this->acceleration += glm::vec2(x,y);
}

void Ball::stop()
{
	this->speed = glm::vec2(0);
	this->isMoving = false;
}

void Ball::update()
{
	this->speed += this->acceleration;

	this->position += this->speed;

	float boundX = Table::bounds.x,
		boundY = Table::bounds.y;
	
	float ballYAbs = abs(this->position.y) + Ball::radius,
		ballXAbs = abs(this->position.x) + Ball::radius;

	// left and right border bounce
	if (ballXAbs >= boundX // is on the border
		&& (ballYAbs <= boundY - Table::hole_bound) // is in Y range of border
		&& !(ballYAbs < Table::hole_bound) // and not in Y range of central hole
		) 
	{

		// oblique line of central hole
		// a = 1/2
		// b = Table::hole_bound - boundX/2

		/*if (ballXAbs > boundX
			&& (ballYAbs <= ballXAbs/2 + Table::hole_bound - boundX/2)
			)
		{
			this->speed.y = Table::bounce_factor;
		}*/
		if (this->borderCollision == false)
		{
			this->borderCollision = true;
			this->speed.x *= Table::bounce_factor;
		}
	}

	// top and bottom border bounce
	if (ballYAbs >= boundY && ballXAbs <= boundX - Table::hole_bound)
	{
		if (this->borderCollision == false)
		{
			this->borderCollision = true;
			this->speed.y *= Table::bounce_factor;
		}
	}

	// central holes bounce
	/*if (ballXAbs >= boundX + Table::hole_bound && ballYAbs < Table::hole_bound)
	{
		this->speed.x *= Table::bounce_factor;
	}*/

	// top holes point
	if(this->position.y >= -this->position.x + boundY + boundX - Table::hole_bound
		|| this->position.y >= this->position.x + boundY + boundX - Table::hole_bound)
	{
		this->visible = false;
		this->stop();
	}

	// bottom holes point
	if (this->position.y <= -this->position.x - boundY - boundX + Table::hole_bound
		|| this->position.y <= this->position.x - boundY - boundX + Table::hole_bound)
	{
		this->visible = false;
		this->stop();
	}

	// central holes point
	if (ballXAbs > boundX + Table::hole_bound/2 && ballYAbs < Table::hole_bound)
	{
		this->visible = false;
		this->stop();
	}

	//angle of rotation
	this->angle -= glm::length(this->speed);
	
	//rotation
	if (this->speed.x == 0 && this->speed.y == 0)
		this->dir = 0;
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
		this->stop();
	else
		this->isMoving = true;
}

bool Ball::collides(Ball* colBall)
{
	if (glm::distance(this->position, colBall->position) <= Ball::radius * 2)
		return true;

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
