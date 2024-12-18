#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace Winedark
{
	class Texture
	{
	private:
		unsigned int		i;
		int					y;

		unsigned int		w;
		unsigned int		h;

		unsigned char*		data;

	public:
		void				SetI(unsigned int i) { this->i = i; }
		void				SetY(int y) { this->y = y; }

		unsigned int		GetI() { return i; }
		int					GetY() { return y; }

		unsigned int		GetWidth() { return w; }
		unsigned int		GetHeight() { return h; }

		unsigned char*		GetData() { return data; }

		Texture(std::string file, unsigned int i);
		Texture(unsigned int width, unsigned int height, unsigned char* data);
	};
}

#endif