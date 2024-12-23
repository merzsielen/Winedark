#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <cstdint>
#include <glm/vec3.hpp>
#include <glad/glad.h>

namespace Winedark::World
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Sparse Voxel Octree																			*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* Voxel																 */
	/*-----------------------------------------------------------------------*/
	/*
		Each voxel is associated with a type ID which tells the renderer
		(and the engine) what kind of block / voxel is there. In addition,
		each voxel has an int which points to the location of its children
		in the voxel array. These children must be contiguous (and there are
		always 8 children to a voxel). If this pointer is negative, then
		this voxel is a leaf.
	*/
	struct Voxel
	{
		uint16_t type;
		int32_t children;
	};

	/*-----------------------------------------------------------------------*/
	/* Octree																 */
	/*-----------------------------------------------------------------------*/
	/*
		Voxels are arranged in an sparse octree.
		  
		    +----+----+		Each voxel (except leaf nodes) features eight
		   /    /    /|		children. These are contiguous in memory and
		  +----+----+ |		may have children of their own. Each child is
		 /    /|   /| +		numbered 0 - 7 based on their relative position.
		+----+----+ |/|
		|    | +  | + |			0 - Front, Bottom, Left
		|    |/	  |/| +			1 - Front, Bottom, Right
		+----+----+ |/			2 - Front, Top, Left
		|    |    | +			3 - Front, Top, Right
		|    |    |/			4 - Back, Bottom, Left
		+----+----+				5 - Back, Bottom, Right
								6 - Back, Top, Left
								7 - Back, Top, Right

		It is important that each voxel is situated exactly like so in
		memory so that we can easily traverse the tree.
	*/
	class Octree
	{
	private:
		/*-----------------------------------------------------*/
		/* Buffer											   */
		/*-----------------------------------------------------*/
		GLuint					ssbo;

		/*-----------------------------------------------------*/
		/* Voxels											   */
		/*-----------------------------------------------------*/
		unsigned int			size;	// Must be multiple of 4.
		double					cx, cy, cz;
		Voxel*					voxels;
		unsigned int			nLayers;
		unsigned int			nVoxels;
		unsigned int			cursor;
		bool					updated;

		/*-----------------------------------------------------*/
		/* Utility											   */
		/*-----------------------------------------------------*/
		std::vector<glm::vec3>	offsets;

		/*-----------------------------------------------------*/
		/* Byffer Functions									   */
		/*-----------------------------------------------------*/
		void					WriteBuffer();

	public:
		/*-----------------------------------------------------*/
		/* General Functions								   */
		/*-----------------------------------------------------*/
		void					Update();

		/*-----------------------------------------------------*/
		/* Voxel Functions									   */
		/*-----------------------------------------------------*/
		void					AddVoxel(unsigned int x, unsigned int y, unsigned int z, uint16_t t);
		void					RemoveVoxel(unsigned int x, unsigned int y, unsigned int z);

		/*-----------------------------------------------------*/
		/* Constructor & Deconstructor						   */
		/*-----------------------------------------------------*/
		Octree(unsigned int size);
		~Octree();
	};
}

#endif