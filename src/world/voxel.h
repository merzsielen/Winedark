#ifndef VOXEL_H
#define VOXEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "../rendering/polygon.h"

namespace Winedark
{
	enum class Face { front, back, bottom, top, left, right };

	bool CheckBit(unsigned char in, unsigned int mask);

	/*------------------------------------------------------------------*/
	/* Voxel                                                            */
	/*------------------------------------------------------------------*/
	/*
		The basic unit of our game world. A voxel is a
		cube which has an associated sprite. In addition,
		each voxel has a set of associated values.

		The "neighbors" byte indicates which neighbors are
		occupied. A	1 is occupied; a 0 is not.

			-------_	front
			------_-	back
			-----_--	bottom
			----_---	top
			---_----	left
			--_-----	right

			00111111	surrounded

		"Checked" works the same way but tells us if we've
		already passed over this face in our greedy
		meshing.
	*/
	struct Voxel
	{
		unsigned char					neighbors;
		unsigned char					checked;
		unsigned short					type;
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
		/* Rendering                                                    */
		/*--------------------------------------------------------------*/
		GLuint							vao;
		GLuint							vbo;

		unsigned int					numTris;
		std::vector<Triangle>			triangles;

		/*--------------------------------------------------------------*/
		/* Parameters                                                   */
		/*--------------------------------------------------------------*/
		unsigned int					i, x, y, z;

		const static unsigned int		width = 20;
		const static unsigned int		height = 20;
		const static unsigned int		depth = 20;

		const static unsigned int		maxTris = (width * height * depth) * 6;

		/*--------------------------------------------------------------*/
		/* Contents                                                     */
		/*--------------------------------------------------------------*/
		Voxel							voxels[width * height * depth];

		/*--------------------------------------------------------------*/
		/* Voxel Functions                                              */
		/*--------------------------------------------------------------*/
		int								CoordToIndex(int x, int y, int z);

		Voxel*							GetVoxel(int i);
		Voxel*							GetVoxel(unsigned int x,
												 unsigned int y,
												 unsigned int z);

		void							CheckVoxel(unsigned int i);

		/*--------------------------------------------------------------*/
		/* Constructor & Deconstructor                                  */
		/*--------------------------------------------------------------*/
		Chunk(unsigned int i, unsigned int x, unsigned int y, unsigned int z);
	};

	/*------------------------------------------------------------------*/
	/* Plane                                                            */
	/*------------------------------------------------------------------*/
	/*
		A "plane" is the visible world, that which may
		need to be rendered. The plane takes all its
		constituent chunks and creates from them a set
		of meshes that it hands to the renderer.
	*/
	class Plane
	{
	private:
		/*--------------------------------------------------------------*/
		/* Spacial Parameters                                           */
		/*--------------------------------------------------------------*/
		/*
			These values should always be odd.
		*/
		const static unsigned int		width = 3;
		const static unsigned int		height = 3;
		const static unsigned int		depth = 3;

		static constexpr float			voxelSize = 1.0f;

		/*--------------------------------------------------------------*/
		/* Chunks                                                       */
		/*--------------------------------------------------------------*/
		Chunk*							centerchunk;
		Chunk*							chunks[width * height * depth];

	public:
		/*--------------------------------------------------------------*/
		/* Chunk Functions                                              */
		/*--------------------------------------------------------------*/
		Chunk*							GetChunk(int i);
		Voxel*							GetVoxel(unsigned int x,
												 unsigned int y,
												 unsigned int z);
		void							CheckVoxels();

		/*--------------------------------------------------------------*/
		/* Greedy Meshing                                               */
		/*--------------------------------------------------------------*/
		/*
			Each of these functions combines matching faces
			across multiple voxels to form the minimum number
			of triangles.
		*/
		void								SweepFace(Face face);

		/*--------------------------------------------------------------*/
		/* Rendering Functions                                          */
		/*--------------------------------------------------------------*/
		void							GenerateMeshes();
		void							Render();

		/*--------------------------------------------------------------*/
		/* Constructor & Deconstructor                                  */
		/*--------------------------------------------------------------*/
		Plane();
	};
}

#endif