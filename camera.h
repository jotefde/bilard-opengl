#include "glm/glm.hpp"

class Camera
{
private:
	float distance = 10.0f;
	float angleAround = 0;
	glm::vec3 position = glm::vec3(0.0f);
	float pitch = 0;
	float yaw = 0;
	float roll = 0;
	glm::vec3 targetPosition = glm::vec3(0.0f);

	float getHorizontalDistance();
	float getVerticalDistance();
	glm::vec3 calculateCameraPosition(float, float);
public:
	Camera();
	glm::mat4 getView();
	void movePitch(float);
	void moveAround(float);
	void setTargetPosition(glm::vec3);
	void moveZ(float);
;	glm::vec3 getTargetPosition();

};

