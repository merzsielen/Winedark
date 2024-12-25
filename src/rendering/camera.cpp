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
	/* Projection & View Functions						   */
	/*-----------------------------------------------------*/
	void Camera::UpdateView()
	{
		this->view = glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), { 0.0f, 1.0f, 0.0f });
	}

	void Camera::UpdateProjection()
	{
		float halfWidth = (width / 2.0f) * zoom;
		float halfHeight = (height / 2.0f) * zoom;

		this->projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearClip, farClip);
	}

	/*-----------------------------------------------------*/
	/* Constructor										   */
	/*-----------------------------------------------------*/
	Camera::Camera(float zoom, glm::vec3 position, Quaternion rotation, unsigned int width, unsigned int height)
	{
		this->zoom = zoom;
		this->position = position;
		this->rotation = rotation;

		this->width = width;
		this->height = height;
	}
}