// main.cpp
//

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <corecrt_math_defines.h>

#include "world/voxel.h"
#include "rendering/renderer.h"

#define VERSION 0.01

void main()
{
	/*
		Let's get some meta-details straight.
		Maw version, OpenGL version, etc.
	*/
	std::ostringstream o;
	o << std::setprecision(2) << std::noshowpoint << VERSION;
	std::cout << "Running Winedark, version: " + o.str() + "." << std::endl;

	/*
		Now we go through the process of initializing OpenGL,
		GLAD, GLFW, GLADOS, GLERP, GLEW, whatever....
	*/
	int windowWidth = 1800;
	int windowHeight = 800;
	std::string title = "Winedark";

	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW." << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHintString(GLFW_X11_CLASS_NAME, "OpenGL");
	glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "OpenGL");

	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		std::cout << "Failed to create Opengl Window." << std::endl;
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/*
		Now we're going to fire up rendering:
		camera, renderer, etc.
	*/
	Winedark::Quaternion iden = { 1.0f, 0.0f, 0.0f, 0.0f };
	float s = M_PI / 8.0f;
	Winedark::Quaternion yRot = { (float)cos(s), 0.0f, (float)sin(s), 0.0f };
	float t = atanf(1 / sqrt(2.0)) / 2.0f;
	Winedark::Quaternion xRot = { -(float)cos(t), (float)sin(t), 0.0f, 0.0f };
	Winedark::Quaternion iso = yRot * xRot;
	Winedark::Quaternion rog = { -(float)cos(s), (float)sin(s), 0.0f, 0.0f };

	Winedark::Camera* camera = new Winedark::Camera({ 0, 0, 0 }, Winedark::NormalizeQuaternion(iden * rog), 0.01f, window);
	Winedark::Renderer* renderer = new Winedark::Renderer(camera);
	Winedark::Plane* plane = new Winedark::Plane();

	/*
		And now we can run the loop.
	*/
	float lastTime = 0.0f;
	auto fpsStart = std::chrono::steady_clock::now();
	int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		float deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		frameCount++;
		auto now = std::chrono::steady_clock::now();
		auto fpsDiff = now - fpsStart;

		if (fpsDiff >= std::chrono::seconds(1))
		{
			fpsStart = now;
			std::cout << "Frame Count: " << frameCount << std::endl;
			frameCount = 0;
		}

		camera->Update(deltaTime);

		renderer->Render(plane);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Shutting down Winedark. Have a wonderful day!" << std::endl;

	delete plane;
	delete renderer;
	delete camera;

}