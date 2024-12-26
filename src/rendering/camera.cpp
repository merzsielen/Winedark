#include "camera.h"

namespace Winedark
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Camera																						*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* Camera																 */
	/*-----------------------------------------------------------------------*/
	/*-----------------------------------------------------*/
	/* Change Check										   */
	/*-----------------------------------------------------*/
	bool Camera::CheckChanged()
	{
		bool c = changed;
		changed = false;
		return c;
	}

	/*-----------------------------------------------------*/
	/* Projection & View Functions						   */
	/*-----------------------------------------------------*/
	void Camera::UpdateView()
	{
		this->view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), { 0.0f, 1.0f, 0.0f });
	}

	void Camera::UpdateProjection()
	{
		float halfWidth = (width / 2.0f) * zoom;
		float halfHeight = (height / 2.0f) * zoom;

		this->projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearClip, farClip);
	}

	/*-----------------------------------------------------*/
	/* General Functions								   */
	/*-----------------------------------------------------*/
	/* Update ---------------------------------------------*/
	void Camera::Update(GLFWwindow* window, float dt)
	{
		HandleInput(window, dt);
		UpdateView();
		UpdateProjection();
	}

	/*---------------------------------------------------*/
	/* TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY */
	/*---------------------------------------------------*/
	/* HandleInput ----------------------------------------*/
	void Camera::HandleInput(GLFWwindow* window, float dt)
	{
		bool moveUp = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
		bool moveDown = ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && !moveUp);
		bool moveLeft = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
		bool moveRight = ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && !moveLeft);
		bool zoomIn = (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS);
		bool zoomOut = ((glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) && !zoomIn);

		if (moveUp)
		{
			position.y += movementSpeed * zoom * dt;
			HasChanged();
		}
		else if (moveDown)
		{
			position.y -= movementSpeed * zoom * dt;
			HasChanged();
		}

		if (moveRight)
		{
			position.x += movementSpeed * zoom * dt;
			HasChanged();
		}
		else if (moveLeft)
		{
			position.x -= movementSpeed * zoom * dt;
			HasChanged();
		}

		if (zoomIn) zoom -= zoomSpeed * dt;
		else if (zoomOut) zoom += zoomSpeed * dt;

		if (zoom < minZoom) zoom = minZoom;
		else if (zoom > maxZoom) zoom = maxZoom;
	}
	/*---------------------------------------------------*/
	/* TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY */
	/*---------------------------------------------------*/

	/*-----------------------------------------------------*/
	/* Constructor										   */
	/*-----------------------------------------------------*/
	Camera::Camera(float zoom, glm::vec3 position, Quaternion rotation, unsigned int width, unsigned int height, float nearClip, float farClip)
	{
		this->changed = false;

		this->zoom = zoom;
		this->position = position;
		this->rotation = rotation;

		this->width = width;
		this->height = height;

		this->nearClip = nearClip;
		this->farClip = farClip;

		this->view = glm::mat4(0);
		this->projection = glm::mat4(0);

		UpdateView();
		UpdateProjection();
	}
}