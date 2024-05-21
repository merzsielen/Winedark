#include "voxel.h"

#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../utility/settings.h"

namespace Winedark
{
	bool CheckBit(unsigned char in, unsigned int mask)
	{
		return (in & mask);
	}

	/*------------------------------------------------------------------*/
	/* Chunk                                                            */
	/*------------------------------------------------------------------*/
	/*--------------------------------------------------------------*/
	/* Voxel Functions                                              */
	/*--------------------------------------------------------------*/
	int Chunk::CoordToIndex(int x, int y, int z)
	{
		if ((x < 0 || x >= width) || (y < 0 || y >= height) || (z < 0 || z >= depth)) return -1;
		return (z * width * height) + (y * height) + x;
	}

	Voxel* Chunk::GetVoxel(int i)
	{
		// Here, we just check if the index is within range.

		if (i < 0 || i >= width * height * depth) return nullptr;

		return &voxels[i];
	}

	Voxel* Chunk::GetVoxel(unsigned int x, unsigned int y, unsigned int z)
	{
		// This is easy. We just need to convert this
		// to an index and grab the voxel that way.

		unsigned int i = x + (y * width) + (z * width * height);
		return GetVoxel(i);
	}

	void Chunk::CheckVoxel(unsigned int i)
	{
		Voxel* t = GetVoxel(i);
		if (t == nullptr) return;

		if (t->type == 0)
		{
			t->checked = (unsigned char)63;
			t->neighbors = (unsigned char)63;
			return;
		}

		int z = i / (width * height);
		int idx = i - (z * width * height);
		int y = idx / width;
		int x = idx % width;

		Voxel* front = GetVoxel(CoordToIndex(x, y, z - 1));
		if (front != nullptr)
		{
			if (front->type != 0)
			{
				t->neighbors |= (unsigned char)1;
				front->neighbors |= (unsigned char)2;
			}
		}

		Voxel* back = GetVoxel(CoordToIndex(x, y, z + 1));
		if (back != nullptr)
		{
			if (back->type != 0)
			{
				t->neighbors |= (unsigned char)2;
				back->neighbors |= (unsigned char)1;
			}
		}

		Voxel* bottom = GetVoxel(CoordToIndex(x, y - 1, z));
		if (bottom != nullptr)
		{
			if (bottom->type != 0)
			{
				t->neighbors |= (unsigned char)4;
				bottom->neighbors |= (unsigned char)8;
			}
		}

		Voxel* top = GetVoxel(CoordToIndex(x, y + 1, z));
		if (top != nullptr)
		{
			if (top->type != 0)
			{
				t->neighbors |= (unsigned char)8;
				top->neighbors |= (unsigned char)4;
			}
		}

		Voxel* left = GetVoxel(CoordToIndex(x - 1, y, z));
		if (left != nullptr)
		{
			if (left->type != 0)
			{
				t->neighbors |= (unsigned char)16;
				left->neighbors |= (unsigned char)32;
			}
		}

		Voxel* right = GetVoxel(CoordToIndex(x + 1, y, z));
		if (right != nullptr)
		{
			if (right->type != 0)
			{
				t->neighbors |= (unsigned char)32;
				right->neighbors |= (unsigned char)16;
			}
		}
	}

	/*--------------------------------------------------------------*/
	/* Constructor & Deconstructor                                  */
	/*--------------------------------------------------------------*/
	Chunk::Chunk(unsigned int i, unsigned int x, unsigned int y, unsigned int z)
	{
		this->i = i;
		this->x = x;
		this->y = y;
		this->z = z;

		this->numTris = 0;

		srand(time(NULL));

		/*-------------------------------------------------*/
		/* Test Tile Setup                                 */
		/*-------------------------------------------------*/
		for (int i = 0; i < (width * height * depth); i++)
		{
			int r = rand() % 100 + 1;

			if (r > 0) voxels[i] = { (unsigned char)0, (unsigned char)0, 1 };
			else voxels[i] = { (unsigned char)0, (unsigned char)0, 0 };
		}

		/*-------------------------------------------------*/
		/* Rendering Setup                                 */
		/*-------------------------------------------------*/
		GLuint ibo;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ARRAY_BUFFER, maxTris * sizeof(Triangle), nullptr, GL_DYNAMIC_DRAW);

		// Coordinates
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
		glEnableVertexAttribArray(1);

		unsigned int indices[maxTris * 3];
		for (int i = 0; i < maxTris; i++)
		{
			const int offset = 3 * i;

			indices[offset + 0] = offset + 0;
			indices[offset + 1] = offset + 1;
			indices[offset + 2] = offset + 2;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	/*------------------------------------------------------------------*/
	/* Plane                                                            */
	/*------------------------------------------------------------------*/
	/*--------------------------------------------------------------*/
	/* Chunk Functions                                              */
	/*--------------------------------------------------------------*/
	Chunk* Plane::GetChunk(int i)
	{
		// We need to check if the index is within the proper range.

		if (i < 0 || i >= width * height * depth) return nullptr;

		return chunks[i];
	}

	Voxel* Plane::GetVoxel(unsigned int x, unsigned int y, unsigned int z)
	{
		// Let's compute some values we need real quick, actually.

		unsigned int half_width = (width - 1) / 2;
		unsigned int half_height = (height - 1) / 2;
		unsigned int half_depth = (depth - 1) / 2;

		// First, we have to figure out what chunk this is in.

		unsigned int chunkX = x / Chunk::width;
		unsigned int chunkY = y / Chunk::height;
		unsigned int chunkZ = z / Chunk::depth;

		int chunkRelX = chunkX - (centerchunk->x - half_width);
		int chunkRelY = chunkY - (centerchunk->y - half_height);
		int chunkRelZ = chunkZ - (centerchunk->z - half_depth);

		// We need to see if this chunk is currently loaded.

		if ((abs((int)(chunkX - centerchunk->x)) > half_width) ||
			(abs((int)(chunkY - centerchunk->y)) > half_height) ||
			(abs((int)(chunkZ - centerchunk->z)) > half_depth)) return nullptr;

		// Our chunk is loaded so we can grab it.

		unsigned int chunkI = chunkRelX + (chunkRelY * width) + (chunkRelZ * width * height);

		Chunk* chunk = GetChunk(chunkI);

		// If, for some reason, this breaks, return nullptr.

		if (chunk == nullptr) return nullptr;

		// Assuming we've got our chunk, we can now
		// calculate the relative coordinates within it.

		unsigned int voxRelX = x - (chunkX * Chunk::width);
		unsigned int voxRelY = y - (chunkY * Chunk::height);
		unsigned int voxRelZ = z - (chunkZ * Chunk::depth);

		return chunk->GetVoxel(voxRelX, voxRelY, voxRelZ);
	}

	void Plane::CheckVoxels()
	{
		unsigned int minChunkX = chunks[0]->x;
		unsigned int minChunkY = chunks[0]->y;
		unsigned int minChunkZ = chunks[0]->z;

		unsigned int minVoxX = minChunkX * Chunk::width;
		unsigned int minVoxY = minChunkY * Chunk::height;
		unsigned int minVoxZ = minChunkZ * Chunk::depth;

		unsigned int xMax = Chunk::width * width;
		unsigned int yMax = Chunk::height * height;
		unsigned int zMax = Chunk::depth * depth;

		for (int x = 0; x < xMax; x++)
		{
			for (int y = 0; y < yMax; y++)
			{
				for (int z = 0; z < zMax; z++)
				{
					unsigned int vx = x + minVoxX;
					unsigned int vy = y + minVoxY;
					unsigned int vz = z + minVoxZ;

					Voxel* vox = GetVoxel(vx, vy, vz);

					if (vox->type == 0)
					{
						vox->checked = (unsigned char)63;
						vox->neighbors = (unsigned char)63;
						continue;
					}

					Voxel* front = GetVoxel(vx, vy, vz - 1);
					if (front != nullptr)
					{
						if (front->type != 0)
						{
							vox->neighbors |= (unsigned char)1;
							front->neighbors |= (unsigned char)2;
						}
					}

					Voxel* back = GetVoxel(vx, vy, vz + 1);
					if (back != nullptr)
					{
						if (back->type != 0)
						{
							vox->neighbors |= (unsigned char)2;
							back->neighbors |= (unsigned char)1;
						}
					}

					Voxel* bottom = GetVoxel(vx, vy - 1, vz);
					if (bottom != nullptr)
					{
						if (bottom->type != 0)
						{
							vox->neighbors |= (unsigned char)4;
							bottom->neighbors |= (unsigned char)8;
						}
					}

					Voxel* top = GetVoxel(vx, vy + 1, vz);
					if (top != nullptr)
					{
						if (top->type != 0)
						{
							vox->neighbors |= (unsigned char)8;
							top->neighbors |= (unsigned char)4;
						}
					}

					Voxel* left = GetVoxel(vx - 1, vy, vz);
					if (left != nullptr)
					{
						if (left->type != 0)
						{
							vox->neighbors |= (unsigned char)16;
							left->neighbors |= (unsigned char)32;
						}
					}

					Voxel* right = GetVoxel(vx + 1, vy, vz);
					if (right != nullptr)
					{
						if (right->type != 0)
						{
							vox->neighbors |= (unsigned char)32;
							right->neighbors |= (unsigned char)16;
						}
					}
				}
			}
		}
	}

	/*--------------------------------------------------------------*/
	/* Greedy Meshing                                               */
	/*--------------------------------------------------------------*/
	void Plane::SweepFace(Face face)
	{
		// First, let's set up our checking thingy.
		unsigned char checkedBit = 0x01;
		checkedBit <<= (unsigned char)((int)face);

		bool frontBack = (face == Face::front || face == Face::back);
		bool bottomTop = (face == Face::bottom || face == Face::top);
		bool leftRight = (face == Face::left || face == Face::right);

		// The chunk at chunk[0] is always the front, bottom,
		// left chunk.
		unsigned int minChunkX = chunks[0]->x;
		unsigned int minChunkY = chunks[0]->y;
		unsigned int minChunkZ = chunks[0]->z;

		unsigned int minVoxX = minChunkX * Chunk::width;
		unsigned int minVoxY = minChunkY * Chunk::height;
		unsigned int minVoxZ = minChunkZ * Chunk::depth;

		// We progress over each layer. If this is front-back,
		// then we progress over the z-layers. If this is left-right,
		// then it is over the x-layers. For top-bottom, it is the
		// y-layers.
		unsigned int iMax = (Chunk::width * width) * (Chunk::height * height);
		unsigned int lMax = Chunk::depth * depth;

		if (bottomTop)
		{
			iMax = (Chunk::width * width) * (Chunk::depth * depth);
			lMax = Chunk::height * height;
		}
		else if (leftRight)
		{
			iMax = (Chunk::height * height) * (Chunk::depth * depth);
			lMax = Chunk::width * width;
		}

		for (unsigned int l = 0; l < lMax; l++)
		{
			for (int i = 0; i < iMax; i++)
			{
				unsigned int x = i % (Chunk::width * width);
				unsigned int y = i / (Chunk::width * width);
				unsigned int z = l;

				if (bottomTop)
				{
					x = i % (Chunk::width * width);
					y = l;
					z = i / (Chunk::width * width);
				}
				else if (leftRight)
				{
					x = l;
					y = i / (Chunk::depth * depth);
					z = i % (Chunk::depth * depth);
				}

				unsigned int chunkX = x / Chunk::width;
				unsigned int chunkY = y / Chunk::height;
				unsigned int chunkZ = z / Chunk::depth;

				unsigned int chunkI = (chunkZ * width * height) + (chunkY * width) + chunkX;

				Chunk* chunk = GetChunk(chunkI);

				unsigned int vx = x + minVoxX;
				unsigned int vy = y + minVoxY;
				unsigned int vz = z + minVoxZ;

				Voxel* vox = GetVoxel(vx, vy, vz);

				// This shouldn't happen, but just in
				// case it does, we should be ready.
				if (vox == nullptr) continue;

				// First, we mark this bit as checked.
				vox->checked |= checkedBit;

				// If this neighbor is blocked, we just don't care.
				if (CheckBit(vox->neighbors, checkedBit)) continue;

				// Temporarily (hopefully), we're just gonna give each
				// face its own mesh.
				glm::vec3 position = glm::vec3(vx * voxelSize, vy * voxelSize, vz * -voxelSize);

				glm::vec4 color = glm::vec4(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f, 1.0f);

				Vertex a, b, c, d;

				float halfWidth = voxelSize / 2.0f;

				if (face == Face::front)
				{
					color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

					float x1 = position.x - halfWidth;
					float x2 = position.x + halfWidth;
					float y1 = position.y - halfWidth;
					float y2 = position.y + halfWidth;
					float z1 = position.z + halfWidth;

					a = { x1, y1, z1, color.r, color.g, color.b, color.a };
					b = { x1, y2, z1, color.r, color.g, color.b, color.a };
					c = { x2, y2, z1, color.r, color.g, color.b, color.a };
					d = { x2, y1, z1, color.r, color.g, color.b, color.a };
				}
				else if (face == Face::back)
				{
					color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);

					float x1 = position.x + halfWidth;
					float x2 = position.x - halfWidth;
					float y1 = position.y + halfWidth;
					float y2 = position.y - halfWidth;
					float z1 = position.z - halfWidth;

					a = { x1, y1, z1, color.r, color.g, color.b, color.a };
					b = { x1, y2, z1, color.r, color.g, color.b, color.a };
					c = { x2, y2, z1, color.r, color.g, color.b, color.a };
					d = { x2, y1, z1, color.r, color.g, color.b, color.a };
				}
				else if (face == Face::bottom)
				{
					color = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);

					float x1 = position.x - halfWidth;
					float x2 = position.x + halfWidth;
					float y1 = position.y - halfWidth;
					float z1 = position.z - halfWidth;
					float z2 = position.z + halfWidth;

					a = { x1, y1, z1, color.r, color.g, color.b, color.a };
					b = { x1, y1, z2, color.r, color.g, color.b, color.a };
					c = { x2, y1, z2, color.r, color.g, color.b, color.a };
					d = { x2, y1, z1, color.r, color.g, color.b, color.a };
				}
				else if (face == Face::top)
				{
					color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

					float x1 = position.x + halfWidth;
					float x2 = position.x - halfWidth;
					float y1 = position.y + halfWidth;
					float z1 = position.z + halfWidth;
					float z2 = position.z - halfWidth;

					a = { x1, y1, z1, color.r, color.g, color.b, color.a };
					b = { x1, y1, z2, color.r, color.g, color.b, color.a };
					c = { x2, y1, z2, color.r, color.g, color.b, color.a };
					d = { x2, y1, z1, color.r, color.g, color.b, color.a };
				}
				else if (face == Face::left)
				{
					color = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);

					float x1 = position.x - halfWidth;
					float y1 = position.y - halfWidth;
					float y2 = position.y + halfWidth;
					float z1 = position.z - halfWidth;
					float z2 = position.z + halfWidth;

					a = { x1, y1, z1, color.r, color.g, color.b, color.a };
					b = { x1, y2, z1, color.r, color.g, color.b, color.a };
					c = { x1, y2, z2, color.r, color.g, color.b, color.a };
					d = { x1, y1, z2, color.r, color.g, color.b, color.a };
				}
				else // if (face == Face::right)
				{
					color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

					float x1 = position.x + halfWidth;
					float y1 = position.y + halfWidth;
					float y2 = position.y - halfWidth;
					float z1 = position.z + halfWidth;
					float z2 = position.z - halfWidth;

					a = { x1, y1, z1, color.r, color.g, color.b, color.a };
					b = { x1, y2, z1, color.r, color.g, color.b, color.a };
					c = { x1, y2, z2, color.r, color.g, color.b, color.a };
					d = { x1, y1, z2, color.r, color.g, color.b, color.a };
				}

				chunk->numTris += 2;
				chunk->triangles.push_back({ a, b, d });
				chunk->triangles.push_back({ b, c, d });

				/*triangles.push_back({ a, b, d });
				triangles.push_back({ b, c, d });*/
			}
		}

		// return triangles;
	}

	/*--------------------------------------------------------------*/
	/* Rendering Functions                                          */
	/*--------------------------------------------------------------*/
	void Plane::GenerateMeshes()
	{
		// Now, we sweep through all six faces of the plane
		// and construct our triangles.
		/*std::vector<Triangle> fronts = SweepFace(Face::front);
		std::vector<Triangle> backs = SweepFace(Face::back);
		std::vector<Triangle> lefts = SweepFace(Face::left);
		std::vector<Triangle> rights = SweepFace(Face::right);
		std::vector<Triangle> tops = SweepFace(Face::top);
		std::vector<Triangle> bottoms = SweepFace(Face::bottom);*/
		SweepFace(Face::front);
		SweepFace(Face::back);
		SweepFace(Face::left);
		SweepFace(Face::right);
		SweepFace(Face::top);
		SweepFace(Face::bottom);

		// Next we reserve the number of batches we'll need.

		/*unsigned int n = fronts.size() + backs.size()
						+ lefts.size() + rights.size()
						+ tops.size() + bottoms.size();

		std::vector<Triangle> triangles;
		triangles.reserve(n);

		unsigned int offset = 0;*/

		/*for (int i = 0; i < fronts.size(); i++) triangles[offset + i] = fronts[i];
		offset += fronts.size();

		for (int i = 0; i < backs.size(); i++) triangles[offset + i] = backs[i];
		offset += backs.size();

		for (int i = 0; i < lefts.size(); i++) triangles[offset + i] = lefts[i];
		offset += lefts.size();

		for (int i = 0; i < rights.size(); i++) triangles[offset + i] = rights[i];
		offset += rights.size();

		for (int i = 0; i < tops.size(); i++) triangles[offset + i] = tops[i];
		offset += tops.size();

		for (int i = 0; i < bottoms.size(); i++) triangles[offset + i] = bottoms[i];
		offset += bottoms.size();*/

		//std::copy(fronts.begin(), fronts.end(), triangles.begin() + offset);
		//offset += fronts.size();

		//std::copy(backs.begin(), backs.end(), triangles.begin() + offset);
		//offset += backs.size();

		//std::copy(lefts.begin(), lefts.end(), triangles.begin() + offset);
		//offset += lefts.size();

		//std::copy(rights.begin(), rights.end(), triangles.begin() + offset);
		//offset += rights.size();

		//std::copy(tops.begin(), tops.end(), triangles.begin() + offset);
		//offset += tops.size();

		//std::copy(bottoms.begin(), bottoms.end(), triangles.begin() + offset);
		//offset += bottoms.size();

		//unsigned int numBatches = 1 + (n / Batch::maxTris);

		//batches.reserve(numBatches);

		//unsigned int filled = 0;

		//for (int i = 0; i < numBatches; i++)
		//{
		//	// And then we go through and fill the batches with
		//	// our triangles.

		//	Batch* b = &batches[i];
		//	b->numTris = std::min(n - filled, Batch::maxTris);
		//	
		//	for (int j = 0; j < b->numTris; j++)
		//	{
		//		b->triangles[j] = triangles[j + filled];
		//	}
		//	// std::copy(triangles.begin() + filled, triangles.begin() + filled + b->numTris, b->triangles);
		//	filled += b->numTris;

		//	// And we also have to set up the vertex array
		//	// objects and all that jazz.

		//	GLuint ibo;

		//	glGenVertexArrays(1, &b->vao);
		//	glBindVertexArray(b->vao);

		//	glGenBuffers(1, &b->vbo);
		//	glBindBuffer(GL_ARRAY_BUFFER, b->vbo);

		//	glGenBuffers(1, &ibo);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		//	glBufferData(GL_ARRAY_BUFFER, b->numTris * sizeof(Triangle), &b->triangles[0], GL_DYNAMIC_DRAW);

		//	// Coordinates
		//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		//	glEnableVertexAttribArray(0);

		//	// Color
		//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
		//	glEnableVertexAttribArray(1);

		//	unsigned int indices[Batch::maxTris * 3];
		//	for (int i = 0; i < Batch::maxTris; i++)
		//	{
		//		const int offset = 3 * i;

		//		indices[offset + 0] = offset + 0;
		//		indices[offset + 1] = offset + 1;
		//		indices[offset + 2] = offset + 2;
		//	}

		//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//	glBindVertexArray(0);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//}

		for (int i = 0; i < (width * height * depth); i++)
		{
			Chunk* c = chunks[i];

			glBindVertexArray(c->vao);
			glBindBuffer(GL_ARRAY_BUFFER, c->vbo);

			glBufferData(GL_ARRAY_BUFFER, c->numTris * sizeof(Triangle), &c->triangles[0], GL_DYNAMIC_DRAW);
			c->triangles.clear();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	void Plane::Render()
	{
		/*
			Here, we run through each batch in turn.
		*/
		//for (int i = 0; i < batches.capacity(); i++)
		//{
		//	Batch* b = &batches[i];

		//	// We must bind the associated arrays for each batch.
		//	glBindVertexArray(b->vao);
		//	glBindBuffer(GL_ARRAY_BUFFER, b->vbo);

		//	// And then call our draw call.
		//	glDrawElements(GL_TRIANGLES, b->numTris * 3, GL_UNSIGNED_INT, nullptr);

		//	// The renderer's Render() function handles the unbinding.
		//}

		for (int i = 0; i < (width * height * depth); i++)
		{
			Chunk* c = chunks[i];

			glBindVertexArray(c->vao);
			glBindBuffer(GL_ARRAY_BUFFER, c->vbo);

			glDrawElements(GL_TRIANGLES, c->numTris * 3, GL_UNSIGNED_INT, nullptr);
		}
	}

	/*--------------------------------------------------------------*/
	/* Constructor & Deconstructor                                  */
	/*--------------------------------------------------------------*/
	Plane::Plane()
	{
		unsigned int size = width * height * depth;

		for (int i = 0; i < size; i++)
		{
			unsigned int z = i / (width * height);
			unsigned int idx = i - (z * width * height);
			unsigned int y = idx / width;
			unsigned int x = idx % width;

			chunks[i] = new Chunk(i, x, y, z);
		}

		unsigned int halfWidth = width / 2;
		unsigned int halfHeight = height / 2;
		unsigned int halfDepth = depth / 2;

		centerchunk = chunks[(halfDepth * width * height) + (halfHeight * width) + halfWidth];
		// if (width == 1 && height == 1 && depth == 1) centerchunk = chunks[0];

		CheckVoxels();

		// GenerateBatches();
		GenerateMeshes();
	}
}