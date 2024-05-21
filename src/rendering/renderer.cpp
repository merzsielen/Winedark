#include "renderer.h"

namespace Winedark
{
	/*-------------------------------------------------*/
	/* Render                                          */
	/*-------------------------------------------------*/
	void Renderer::Render(Plane* plane)
	{
		shaders[0].Use();
		shaders[0].SetMatrix("MVP", camera->GetViewProjection());

		plane->Render();

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	/*-------------------------------------------------*/
	/* Constructor & Deconstructor                     */
	/*-------------------------------------------------*/
	Renderer::Renderer(Camera* camera)
	{
		this->camera = camera;

		Shader baseShader = { "assets/shaders/base.vert", "assets/shaders/base.frag" };
		shaders.push_back(baseShader);
	}
}