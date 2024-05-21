#include "camera.h"

#include <glm/gtx/quaternion.hpp>

#include "../utility/settings.h"

namespace Winedark
{
	/*-----------------------------------------------*/
	/* Temporary Input Handling */
	/*-----------------------------------------------*/
	void Camera::HandleInput(float deltaTime)
	{
		glm::vec3 forward = CameraForward();
		glm::vec3 up = CameraUp();
		glm::vec3 right = CameraRight();

		/*
			MOVEMENT
		*/
		bool moveRight = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
		bool moveLeft = ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && !moveRight);
		bool moveUp = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
		bool moveDown = ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && !moveUp);
		bool moveIn = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
		bool moveOut = ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) && !moveIn);

		if (moveRight) position += right * deltaTime * movementSpeed;
		else if (moveLeft) position -= right * deltaTime * movementSpeed;

		if (moveUp) position += up * deltaTime * movementSpeed;
		else if (moveDown) position -= up * deltaTime * movementSpeed;

		if (moveIn) position += forward * deltaTime * movementSpeed;
		else if (moveOut) position -= forward * deltaTime * movementSpeed;

		/*
			ROTATION
		*/
		bool lookRight = (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS);
		bool lookLeft = ((glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) && !lookRight);
		bool lookUp = (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS);
		bool lookDown = ((glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) && !lookUp);

		if (lookRight) rotation = Slerp(rotation, rotation * Quaternion(deltaTime, 0.0f, 1.0f, 0.0f), deltaTime);
		else if (lookLeft) rotation = Slerp(rotation, rotation * Quaternion(deltaTime, 0.0f, -1.0f, 0.0f), deltaTime);

		if (lookUp) rotation = Slerp(rotation, rotation * Quaternion(deltaTime, -1.0f, 0.0f, 0.0f), deltaTime);
		else if (lookDown) rotation = Slerp(rotation, rotation * Quaternion(deltaTime, 1.0f, 0.0f, 0.0f), deltaTime);

		/*
			ZOOM
		*/
		bool zoomIn = (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS);
		bool zoomOut = ((glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) && !zoomIn);

		if (zoomIn) zoom -= deltaTime * zoomSpeed;
		else if (zoomOut) zoom += deltaTime * zoomSpeed;
	}

	/*-----------------------------------------------*/
	/* Projection & View Functions */
	/*-----------------------------------------------*/
	void Camera::UpdateProjection()
	{
		if (lens == Lens::orthographic)
		{
			float halfWidth = (windowWidth / 2.0f) * zoom;
			float halfHeight = (windowHeight / 2.0f) * zoom;

			projection = glm::ortho(-halfWidth, halfWidth, -halfWidth, halfHeight, nearClip, farClip);
		}
		else
		{
			projection = glm::perspective(fov, (windowWidth / (float)windowHeight), nearClip, farClip);
		}
	}

	void Camera::UpdateView()
	{
		view = glm::lookAt(position, position + CameraForward(), CameraUp());
		// if (lens == Lens::perspective) view = glm::inverse(view);
	}

	/*-----------------------------------------------*/
	/* Update (Main Loop) */
	/*-----------------------------------------------*/
	void Camera::Update(float deltaTime)
	{
		HandleInput(deltaTime);

		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		UpdateProjection();
		UpdateView();

		UpdateRotation();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearDepth(1.0);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	/*-----------------------------------------------*/
	/* Constructor & Deconstructor */
	/*-----------------------------------------------*/
	Camera::Camera(glm::vec3 position, Quaternion rotation, float zoom, GLFWwindow* window)
	{
		this->position = position;
		this->rotation = rotation;
		this->zoom = zoom;

		this->fov = Settings::FOV;
		this->nearClip = Settings::NearClip;
		this->farClip = Settings::FarClip;

		this->movementSpeed = Settings::CameraMovementSpeed;
		this->zoomSpeed = Settings::CameraZoomSpeed;

		this->backgroundColor = Settings::BackgroundColor;

		this->window = window;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		UpdateView();
		UpdateProjection();

		this->position -= CameraForward() * 200.0f;
	}

	/*Camera::~Camera()
	{
		
	}*/
}