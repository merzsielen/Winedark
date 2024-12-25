#ifndef TEXTURE_H
#define TEXTURE_H

namespace Winedark
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Texture																						*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* Texture																 */
	/*-----------------------------------------------------------------------*/
	class Texture
	{
	private:
		/*---------------------------------------------------*/
		/* Dimensions										 */
		/*---------------------------------------------------*/
		int						width;
		int						height;

		/*---------------------------------------------------*/
		/* Data												 */
		/*---------------------------------------------------*/
		unsigned char*			data;

		/*---------------------------------------------------*/
		/* Indices											 */
		/*---------------------------------------------------*/
		int						atlasIndex;

	public:
		/*---------------------------------------------------*/
		/* Dimension Functions								 */
		/*---------------------------------------------------*/
		int						GetWidth() { return width; }
		int						GetHeight() { return height; }

		/*---------------------------------------------------*/
		/* Data Functions									 */
		/*---------------------------------------------------*/
		unsigned char*			GetData() { return data; }

		/*---------------------------------------------------*/
		/* Index Functions									 */
		/*---------------------------------------------------*/
		int						GetAtlasIndex() { return atlasIndex; }
		void					SetAtlasIndex(int i) { this->atlasIndex = i; }

		/*---------------------------------------------------*/
		/* Constructors										 */
		/*---------------------------------------------------*/
		Texture(const char* path);
		// ...
	};
}

#endif