#ifndef ARCHTEXTURE_H
#define ARCHTEXTURE_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

namespace Winedark
{
	class Archtexture
	{
	private:
		GLint			id;
		unsigned int	width;
		unsigned int	height;

		void			AddWidth(unsigned int w) { width = std::max(w, width); }
		void			AddHeight(unsigned int h) { height += h; }

	public:
		GLint			GetID() { return id; }
		void			Prepare(std::vector<Texture*> textures);

		unsigned int	GetWidth() { return width; }
		unsigned int	GetHeight() { return height; }
	};
}

#endif