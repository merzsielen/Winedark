#include "texture.h"

#include <stb_image.h>

namespace Winedark
{
	Texture::Texture(std::string file, unsigned int i)
	{
		this->i = i;
		this->y = -1;

		stbi_set_flip_vertically_on_load(true);
		int imageWidth, imageHeight, nrChannels;
		data = stbi_load(file.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);

		this->w = imageWidth;
		this->h = imageHeight;
	}

	Texture::Texture(unsigned int width, unsigned int height, unsigned char* data)
	{
		this->i = 0;
		this->y = -1;
		this->w = width;
		this->h = height;
		this->data = data;
	}
}