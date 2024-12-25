#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../util/geometry.h"

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
	/*
		The camera handles the properties and movement of the camera in
		the game-world.
	*/
	class Camera
	{
	private:
		/*-----------------------------------------------------*/
		/* Zoom, Position, & Rotation						   */
		/*-----------------------------------------------------*/
		float				zoom;
		glm::vec3			position;
		Quaternion			rotation;

		/*-----------------------------------------------------*/
		/* Position & Rotation								   */
		/*-----------------------------------------------------*/
		float				nearClip;
		float				farClip;

		/*-----------------------------------------------------*/
		/* Dimensions										   */
		/*-----------------------------------------------------*/
		unsigned int		width;
		unsigned int		height;

		/*-----------------------------------------------------*/
		/* Projection & View								   */
		/*-----------------------------------------------------*/
		glm::mat4			view;
		glm::mat4			projection;

	public:
		/*-----------------------------------------------------*/
		/* Position & Rotation Functions					   */
		/*-----------------------------------------------------*/
		glm::vec3			GetPosition() { return position; }
		Quaternion			GetRotation() { return rotation; }

		/*-----------------------------------------------------*/
		/* Position & Rotation Functions					   */
		/*-----------------------------------------------------*/
		float				GetNearClip() { return nearClip; }
		float				GetFarClip() { return farClip; }

		/*-----------------------------------------------------*/
		/* Dimensions Functions								   */
		/*-----------------------------------------------------*/
		unsigned int		GetWidth() { return width; }
		unsigned int		GetHeight() { return height; }

		/*-----------------------------------------------------*/
		/* Projection & View Functions						   */
		/*-----------------------------------------------------*/
		glm::mat4			GetView() { return view; }
		glm::mat4			GetProjection() { return projection; }
		glm::mat4			GetViewProjection() { return projection * view; }

		void				UpdateView();
		void				UpdateProjection();

		/*-----------------------------------------------------*/
		/* Constructor										   */
		/*-----------------------------------------------------*/
		Camera(float zoom, glm::vec3 position, Quaternion rotation, unsigned int width, unsigned int height);
	};
}

#endif