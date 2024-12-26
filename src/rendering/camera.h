#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
		/* Flags											   */
		/*-----------------------------------------------------*/
		bool				changed;

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

		/*-----------------------------------------------------*/
		/* Flag Functions 1									   */
		/*-----------------------------------------------------*/
		void				HasChanged() { changed = true; };

		/*---------------------------------------------------*/
		/* TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY */
		/*---------------------------------------------------*/
		/*-----------------------------------------------------*/
		/* Movement & Zoom									   */
		/*-----------------------------------------------------*/
		float				movementSpeed = 1000.0f;
		float				zoomSpeed = 0.5f;

		float				minZoom = 0.001f;
		float				maxZoom = 10.0f;
		/*---------------------------------------------------*/
		/* TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY */
		/*---------------------------------------------------*/

	public:
		/*-----------------------------------------------------*/
		/* Flag Functions 2									   */
		/*-----------------------------------------------------*/
		bool				CheckChanged();

		/*-----------------------------------------------------*/
		/* General Functions								   */
		/*-----------------------------------------------------*/
		void				Update(GLFWwindow* window, float dt);
		void				HandleInput(GLFWwindow* window, float dt);

		/*-----------------------------------------------------*/
		/* Position & Rotation Functions					   */
		/*-----------------------------------------------------*/
		glm::vec3			GetPosition() { return position; }
		Quaternion			GetRotation() { return rotation; }
		float				GetZoom() { return zoom; }

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
		Camera(float zoom, glm::vec3 position, Quaternion rotation, unsigned int width, unsigned int height, float nearClip, float farClip);
	};
}

#endif