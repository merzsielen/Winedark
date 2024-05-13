#ifndef VOXEL_H
#define VOXEL_H

namespace Winedark
{
	/*------------------------------------------------------------------*/
	/* Voxel                                                            */
	/*------------------------------------------------------------------*/
	/*
		The basic unit of our game world. A voxel is a
		cube which has an associated sprite. In addition,
		each voxel has a set of associated values.
	*/
	struct Voxel
	{
		unsigned short					textureIndex;
		unsigned char					masks;
		unsigned char					depth;
		unsigned char					children;
	};

	/*------------------------------------------------------------------*/
	/* Chunk                                                            */
	/*------------------------------------------------------------------*/
	/*
		A chunk is a section of the world which is loaded
		or unloaded depending on the position of the
		camera / player character.
	*/
	struct Chunk
	{
		/*--------------------------------------------------------------*/
		/* Parameters                                                   */
		/*--------------------------------------------------------------*/
		const static unsigned int		width = 64;
		const static unsigned int		height = 64;
		const static unsigned int		depth = 64;

		/*--------------------------------------------------------------*/
		/* Contents                                                     */
		/*--------------------------------------------------------------*/
		Voxel							voxels[width * height * depth];
	};

	/*------------------------------------------------------------------*/
	/* Plane                                                            */
	/*------------------------------------------------------------------*/
	/*
		A "plane" is the visible world, that which may
		need to be rendered. The plane takes all its
		constituent chunks and compresses them down into
		an octree.
	*/
	class Plane
	{
	private:
		/*--------------------------------------------------------------*/
		/* Parameters                                                   */
		/*--------------------------------------------------------------*/
		const static unsigned int		width = 8;
		const static unsigned int		height = 8;
		const static unsigned int		depth = 8;

		/*--------------------------------------------------------------*/
		/* Chunks                                                       */
		/*--------------------------------------------------------------*/
		Chunk*							chunks[width * height * depth];

		/*--------------------------------------------------------------*/
		/* GPU                                                          */
		/*--------------------------------------------------------------*/
		void							Render();
	};
}

#endif