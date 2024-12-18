#include "archtexture.h"

namespace Winedark
{
	void Archtexture::Prepare(std::vector<Texture*> textures)
	{
		// First, we reset the width and height.
		width = 0;
		height = 0;

		// Now, we add each texture.
		for (int i = 0; i < textures.size(); i++)
		{
			Texture* t = textures[i];
			t->SetY(height);
			AddWidth(t->GetWidth());
			AddHeight(t->GetHeight());
		}

		// Now we bind the archtexture.
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// And now we add in each texture's data.
		for (int i = 0; i < textures.size(); i++)
		{
			Texture* t = textures[i];
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, t->GetY(), t->GetWidth(), t->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, t->GetData());
		}

		// Next we set the wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// And finally we unbind.
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}