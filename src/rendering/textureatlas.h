#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

namespace Winedark
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Texture Atlas																				*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* TextureAtlas															 */
	/*-----------------------------------------------------------------------*/
	/*
		The texture atlas holds the data of all the active textures.
		Whenever a new texture is added or an old texture ceases to
		be used, we rebuild the texture atlas. This allows us to only
		ever bind one texture (the atlas) rather than having to handle
		binding different textures for different batches.
	*/
	class TextureAtlas
	{
	private:
		/*---------------------------------------------------*/
		/* ID												 */
		/*---------------------------------------------------*/
		GLint				id;

		/*---------------------------------------------------*/
		/* Dimensions										 */
		/*---------------------------------------------------*/
		int					width;
		int					height;

	public:
		/*---------------------------------------------------*/
		/* ID Functions										 */
		/*---------------------------------------------------*/
		GLint				GetID() { return id; }

		/*---------------------------------------------------*/
		/* Dimension Functions								 */
		/*---------------------------------------------------*/
		int					GetWidth() { return width; }
		int					GetHeight() { return height; }

		void				AddWidth(int w);
		void				AddHeight(int h);

		/*---------------------------------------------------*/
		/* Build											 */
		/*---------------------------------------------------*/
		void				Build(std::vector<Texture*> textures);

		/*---------------------------------------------------*/
		/* Constructor										 */
		/*---------------------------------------------------*/
		// TextureAtlas();
	};
}

#endif