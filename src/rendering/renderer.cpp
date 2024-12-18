#include "renderer.h"

namespace Winedark
{
	/*-----------------------------------------------*/
	/* Texture Functions */
	/*-----------------------------------------------*/
	void Renderer::AddTexture(std::string path)
	{
		Texture t = Texture(path, ++nextTexID);
		textures[t.GetI()] = t;
	}

	/*-------------------------------------------------*/
	/* Render                                          */
	/*-------------------------------------------------*/
	void Renderer::Render(Plane* plane)
	{
		shaders[0].Use();
		shaders[0].SetMatrix("MVP", camera->GetViewProjection());
		shaders[0].SetVector2("AS", glm::vec2(archtexture->GetWidth(), archtexture->GetHeight()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, archtexture->GetID());

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
		this->nextTexID = 0;
		this->camera = camera;

		Shader baseShader = { "assets/shaders/base.vert", "assets/shaders/base.frag" };
		shaders.push_back(baseShader);

		// This will maybe need fixing.
		glUniform1iv(glGetUniformLocation(baseShader.ID, "batchTextures"), 1, { 0 });
		archtexture = new Archtexture();
	}
}