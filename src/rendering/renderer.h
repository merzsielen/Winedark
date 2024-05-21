#ifndef RENDERER_H
#define RENDERER_H

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
		Camera*							camera;

		/*-----------------------------------------------*/
		/* Shaders */
		/*-----------------------------------------------*/
		std::vector<Shader>				shaders;

	public:
		/*-----------------------------------------------*/
		/* Render */
		/*-----------------------------------------------*/
		void							Render(Plane* plane);

		/*-----------------------------------------------*/
		/* Constructor & Deconstructor */
		/*-----------------------------------------------*/
		Renderer(Camera* camera);
	};
}

#endif