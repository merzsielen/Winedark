#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "shader.h"
#include "textureatlas.h"
#include "../world/octree.h"
#include "../util/polygons.h"

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
	/*
		Okay, so the renderer needs to take the camera and the octree and
		send information about the former's position, rotation, and zoom
		to a compute shader which traverses the octree in order to generate
		a texture representing the view of the voxel world from the camera.

		This requires a:
			- compute shader
			- scene shader
			- scene texture
			- scene triangles (2)
		
		Each frame, (if texture needs to be updated) we run the compute shader
		to update the scene texture, then pass that to the GPU and tell it
		to render the scene.
	*/
	class Renderer
	{
	private:
		/*-------------------------------------------------------*/
		/* Buffers												 */
		/*-------------------------------------------------------*/
		GLuint					vao;
		GLuint					vbo;

		/*-------------------------------------------------------*/
		/* Screen Quad											 */
		/*-------------------------------------------------------*/
		Quad					screenQuad;

		/*-------------------------------------------------------*/
		/* Camera												 */
		/*-------------------------------------------------------*/
		Camera*					camera;

		/*-------------------------------------------------------*/
		/* Octree												 */
		/*-------------------------------------------------------*/
		Octree*					octree;
		GLuint					octreeTexture;

		/*-------------------------------------------------------*/
		/* Shaders												 */
		/*-------------------------------------------------------*/
		Shader					sceneShader;
		Shader					computeShader;

		/*-------------------------------------------------------*/
		/* Textures												 */
		/*-------------------------------------------------------*/
		std::vector<Texture>	textures;
		TextureAtlas			textureAtlas;

	public:
		/*-------------------------------------------------------*/
		/* Rendering Functions									 */
		/*-------------------------------------------------------*/
		bool					IsUpdateNeeded();
		void					Render();

		/*-------------------------------------------------------*/
		/* Constructor											 */
		/*-------------------------------------------------------*/
		Renderer(Camera* camera, Octree* octree);
	};
}

#endif