#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>

#include "archtexture.h"
#include "../world/voxel.h"
#include "camera.h"
#include "shader.h"

namespace Winedark
{
	/*------------------------------------------------------------------*/
	/* Renderer                                                         */
	/*------------------------------------------------------------------*/
	class Renderer
	{
	private:
		/*-----------------------------------------------*/
		/* Camera */
		/*-----------------------------------------------*/
		Camera*										camera;

		/*-----------------------------------------------*/
		/* Textures */
		/*-----------------------------------------------*/
		unsigned int								nextTexID;
		Archtexture*								archtexture;
		std::unordered_map<unsigned int, Texture>	textures;

		/*-----------------------------------------------*/
		/* Shaders */
		/*-----------------------------------------------*/
		std::vector<Shader>							shaders;

	public:
		/*-----------------------------------------------*/
		/* Texture Functions */
		/*-----------------------------------------------*/
		void										AddTexture(std::string path);

		/*-----------------------------------------------*/
		/* Render */
		/*-----------------------------------------------*/
		void										Render(Plane* plane);

		/*-----------------------------------------------*/
		/* Constructor & Deconstructor */
		/*-----------------------------------------------*/
		Renderer(Camera* camera);
	};
}

#endif