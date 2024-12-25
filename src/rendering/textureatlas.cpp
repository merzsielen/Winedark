#include "textureatlas.h"

#include <algorithm>

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
	/*---------------------------------------------------*/
	/* Dimension Functions								 */
	/*---------------------------------------------------*/
	/*
		The dimension functions just handle how width
		and height is added to the texture atlas.
		These are straightforward enough that I'll
		waive further explanation.
	*/
	void TextureAtlas::AddWidth(int w)
	{
		this->width = std::max(w, width);
	}

	void TextureAtlas::AddHeight(int h)
	{
		this->height += h;
	}

	/*---------------------------------------------------*/
	/* Build											 */
	/*---------------------------------------------------*/
	void TextureAtlas::Build(std::vector<Texture*> textures)
	{
		/*
			First, we reset the width and height.
		*/
		width = 0;
		height = 0;

		/*
			Next, we cycle through the active textures
			and add them to the texture atlas.
		*/
		for (int i = 0; i < textures.size(); i++)
		{
			Texture* t = textures[i];
			t->SetAtlasIndex(height);
			AddWidth(t->GetWidth());
			AddHeight(t->GetHeight());
		}

		/*
			Then we create the texture and
			fill its data. Sadly, this means looping
			over all active textures again.
		*/
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		for (int i = 0; i < textures.size(); i++)
		{
			Texture* t = textures[i];
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, t->GetAtlasIndex(), t->GetWidth(), t->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, t->GetData());
		}

		/*
			Finally, we set the wrap and filter modes and
			unbind the texture.
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/*---------------------------------------------------*/
	/* Constructor										 */
	/*---------------------------------------------------*/
	/*TextureAtlas::TextureAtlas()
	{

	}*/
}