#include "Camera.h"

void Camera::setPerspective(float fieldOfView, float aspectRatio, float Near, float Far)
{
	projectionTransform = glm::perspective(fieldOfView, aspectRatio, Near, Far);
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	viewTransform = glm::lookAt(from, to, up);
}

void Camera::setPosition(vec3 position)
{
	worldTransform = worldTransform * glm::translate(position);
}

mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

mat4 Camera::getView()
{
	return viewTransform;
}

mat4 Camera::getProjection()
{
	return projectionTransform;
}

mat4 Camera::getProjectionView()
{
	updateProjectionViewTransform();
	return projectionViewTransform;
}

void Camera::updateProjectionViewTransform()
{
	projectionViewTransform = projectionTransform * viewTransform * worldTransform;
}

void FlyCamera::update(float deltaTime, GLFWwindow* window)
{
	vec3 movement = vec3(0);

	int W = glfwGetKey(window, GLFW_KEY_W);
	int A = glfwGetKey(window, GLFW_KEY_A);
	int S = glfwGetKey(window, GLFW_KEY_S);
	int D = glfwGetKey(window, GLFW_KEY_D);

	if (W == GLFW_PRESS)
	{
		movement.x += speed * deltaTime;
		movement.z += speed * deltaTime;
	}
	if (A == GLFW_PRESS)
	{
		movement.x += speed * deltaTime;
		movement.z -= speed * deltaTime;
	}
	if (S == GLFW_PRESS)
	{
		movement.x -= speed * deltaTime;
		movement.z -= speed * deltaTime;
	}
	if (D == GLFW_PRESS)
	{
		movement.x -= speed * deltaTime;
		movement.z += speed * deltaTime;
	}
	worldTransform *= glm::translate(movement);
	getProjectionView();

}

void FlyCamera::setSpeed(float speed)
{
	this->speed = speed;
}