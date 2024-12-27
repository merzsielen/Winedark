#include "renderer.h"

#include <iostream>

namespace Winedark
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Rendering																					*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* Renderer																 */
	/*-----------------------------------------------------------------------*/
	/*-------------------------------------------------------*/
	/* Rendering Functions									 */
	/*-------------------------------------------------------*/
	bool Renderer::IsUpdateNeeded()
	{
		return (camera->CheckChanged() || octree->CheckChanged());
	}

	void Renderer::Render()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, octreeTexture);

		if (IsUpdateNeeded())
		{
			octree->OverwriteBufferData();

			computeShader.Use();
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, octree->GetSSBO());
			glDispatchCompute(camera->GetWidth(), camera->GetHeight(), 1);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		};

		sceneShader.Use();
		SetMatrix(sceneShader.GetID(), "mvp", camera->GetViewProjection());

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Quad), &screenQuad);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	/*-------------------------------------------------------*/
	/* Constructor											 */
	/*-------------------------------------------------------*/
	Renderer::Renderer(Camera* camera, Octree* octree) :
		sceneShader("assets/shaders/base.vert", "assets/shaders/base.frag"),
		computeShader("assets/shaders/base.comp")
	{
		/*
			First, some preliminary pointers.
		*/
		this->camera = camera;
		this->octree = octree;

		/*
			Next, we set up our OpenGL buffers.
		*/
		GLuint ibo;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), nullptr, GL_DYNAMIC_DRAW);

		/*
			Now we need to assign our vertex attribute
			pointers. These include spatial coordinates,
			color, and texture coordinates.
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(2);

		/*
			Next, we need to set up our indices data.
			
			We may very well need to change this when
			we add more stuff to the renderer.
		*/
		const unsigned int nTriangles = 2;
		const unsigned int nVertices = 3 * nTriangles;

		unsigned int indices[nVertices];
		for (int i = 0; i < nVertices; i += 3)
		{
			indices[i] = i;
			indices[i + 1] = i + 1;
			indices[i + 2] = i + 2;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		/*
			Finally, we need to prepare octree texture
			and the screen quad on which we project it.
		*/
		glGenTextures(1, &octreeTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, octreeTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, camera->GetWidth(), camera->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindImageTexture(0, octreeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		// Now the screen quad.
		float hw = (float)camera->GetWidth() / 2.0f;
		float hh = (float)camera->GetHeight() / 2.0f;

		screenQuad =
		{
			{
				{ -hw, -hh, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
				{ -hw, hh, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
				{ hw, -hh, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }
			},
			{
				{ -hw, hh, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
				{ hw, hh, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
				{ hw, -hh, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }
			}
		};

		// And the texture atlas.
		// ...

		// Oh, and the texture samplers.
		glUseProgram(sceneShader.GetID());
		GLint location = glGetUniformLocation(sceneShader.GetID(), "textures");
		int samplers[1] = { 0 };
		glUniform1iv(location, 1, samplers);
	}
}