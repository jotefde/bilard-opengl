#include "camera.h"
#include <glm/gtc/type_ptr.hpp>

float Camera::getHorizontalDistance()
{
	return this->distance * cosf(this->pitch);
}

float Camera::getVerticalDistance()
{
	return this->distance * sinf(this->pitch);
}

glm::vec3 Camera::calculateCameraPosition(float horizDist, float vertDist)
{
	float offsetX = horizDist * sinf(angleAround);
	float offsetY = horizDist * cosf(angleAround);
	return glm::vec3(
		targetPosition.x - offsetX,
		targetPosition.y - offsetY,
		targetPosition.z + vertDist
	);
}

Camera::Camera()
{
}

glm::mat4 Camera::getView()
{
	float horizDist = this->getHorizontalDistance();
	float vertDist = this->getVerticalDistance();
	this->position = this->calculateCameraPosition(horizDist, vertDist);
	return glm::lookAt(
		this->position,
		this->targetPosition,
		glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz widoku
}

void Camera::movePitch(float pChange)
{
	this->pitch += pChange;
}

void Camera::moveAround(float aroundChange)
{
	this->angleAround -= aroundChange;
}

void Camera::setTargetPosition(glm::vec3 pos)
{
	this->targetPosition = pos;
}

void Camera::moveZ(float dist)
{
	this->distance += dist;
}

glm::vec3 Camera::getTargetPosition()
{
	return this->targetPosition;
}
