#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../utility/geometry.h"

namespace Winedark
{
	enum class Lens { orthographic, perspective };

	class Camera
	{
	private:
		/*-----------------------------------------------*/
		/* Position, Rotation, & Zoom                    */
		/*-----------------------------------------------*/
		glm::vec3			position;
		Quaternion			rotation;
		float				zoom;

		/*-----------------------------------------------*/
		/* Window, FOV, Clip                             */
		/*-----------------------------------------------*/
		GLFWwindow* window;
		int					windowWidth;
		int					windowHeight;

		float				fov;
		float				nearClip;
		float				farClip;

		/*-----------------------------------------------*/
		/* Speed & Distance */
		/*-----------------------------------------------*/
		float				movementSpeed;
		float				zoomSpeed;

		/*-----------------------------------------------*/
		/* Projection & View */
		/*-----------------------------------------------*/
		Lens				lens = Lens::orthographic;

		glm::mat4			view;
		glm::mat4			projection;

		/*-----------------------------------------------*/
		/* Background Color */
		/*-----------------------------------------------*/
		glm::vec4			backgroundColor;

		/*-----------------------------------------------*/
		/* Input Handling */
		/*-----------------------------------------------*/
		void				HandleInput(float deltaTime);

	public:
		/*-----------------------------------------------*/
		/* Position & Rotation Functions */
		/*-----------------------------------------------*/
		glm::vec3			GetPosition() { return position; }
		Quaternion			GetRotation() { return rotation; }
		float				GetZoom() { return zoom; }

		void				SetPosition(glm::vec3 pos) { position = pos; }
		void				SetRotation(Quaternion quat) { rotation = quat; }
		void				SetZoom(float z) { zoom = z; }

		void				UpdateRotation() { NormalizeQuaternion(rotation); }
		glm::vec3			CameraRight() { return Rotate({ 1.0f, 0.0f, 0.0f }, rotation); }
		glm::vec3			CameraForward() { return Rotate({ 0.0f, 0.0f, -1.0f }, rotation); }
		glm::vec3			CameraUp() { return Rotate({ 0.0f, 1.0f, 0.0f }, rotation); }

		/*-----------------------------------------------*/
		/* Window & Clip Functions */
		/*-----------------------------------------------*/
		int					GetWindowWidth() { return windowWidth; }
		int					GetWindowHeight() { return windowHeight; }

		void				SetWindowWidth(int width) { windowWidth = width; }
		void				SetWindowHeight(int height) { windowHeight = height; }

		/*-----------------------------------------------*/
		/* Projection & View Functions */
		/*-----------------------------------------------*/
		Lens				GetLens() { return lens; }

		void				SetLens(Lens l) { lens = l; }

		glm::mat4			GetViewProjection() { return projection * view; }

		void				UpdateView();
		void				UpdateProjection();

		/*-----------------------------------------------*/
		/* Background Color Functions */
		/*-----------------------------------------------*/
		glm::vec4			GetBackgroundColor() { return backgroundColor; }

		void				SetBackgroundColor(glm::vec4 color) { backgroundColor = color; }

		/*-----------------------------------------------*/
		/* Update (Main Loop) */
		/*-----------------------------------------------*/
		void				Update(float deltaTime);

		/*-----------------------------------------------*/
		/* Constructor & Deconstructor */
		/*-----------------------------------------------*/
		Camera(glm::vec3 position, Quaternion rotation, float zoom, GLFWwindow* window);

		// ~Camera();
	};
}

#endif