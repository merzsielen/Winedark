#include "octree.h"

#include <cmath>
#include <corecrt_malloc.h>
#include <time.h>
#include <iostream>

namespace Winedark
{
	/*----------------------------------------------------------------------------------------------*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/* Sparse Voxel Octree																			*/
	/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
	/*----------------------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------*/
	/* Octree																 */
	/*-----------------------------------------------------------------------*/
	/*---------------------------------------------------*/
	/* Utility Functions								 */
	/*---------------------------------------------------*/
	/* Diff2Oct -----------------------------------------*/
	/*
		Diff2Oct (Difference to Octant) takes the difference
		between two points (taken to be the middle of a voxel
		and some position within its children) and returns
		the child voxel in which that point lies.

		Input: Difference between two points (dx, dy, dz)
		Output: Index / Offset of the child voxel.
	*/
	unsigned int Diff2Oct(glm::vec3 d)
	{
		if (d.z < 0 && d.y < 0 && d.x < 0)	return 0;
		if (d.z < 0 && d.y < 0 && d.x > 0) return 1;
		if (d.z < 0 && d.y > 0 && d.x < 0) return 2;
		if (d.z < 0 && d.y > 0 && d.x > 0) return 3;
		if (d.z > 0 && d.y < 0 && d.x < 0)	return 4;
		if (d.z > 0 && d.y < 0 && d.x > 0) return 5;
		if (d.z > 0 && d.y > 0 && d.x < 0) return 6;
		if (d.z > 0 && d.y > 0 && d.x > 0) return 7;
		return 0;
	}

	/*-----------------------------------------------------*/
	/* Flag Functions									   */
	/*-----------------------------------------------------*/
	bool Octree::CheckChanged()
	{
		bool c = changed;
		changed = false;
		return c;
	}

	/*---------------------------------------------------*/
	/* General Functions								 */
	/*---------------------------------------------------*/
	/* Update -------------------------------------------*/
	/*
		Update currently checks if the octree has been
		updated and, if so, writes it to the buffer.
	*/
	void Octree::Update()
	{
		if (updated)
		{
			updated = false;
			WriteBuffer();
			HasChanged();
		}
	}

	/* OverwriteBufferData ------------------------------*/
	/*
		Just overwrites the BufferData in the ssbo
		without touching the voxels.
	*/
	void Octree::OverwriteBufferData()
	{
		Quaternion r = camera->GetRotation();
		glm::vec3 right = Rotate({ 1.0, 0.0,0.0 }, r);
		glm::vec3 up = Rotate({ 0.0, 1.0,0.0 }, r);
		glm::vec3 forward = Rotate({ 0.0, 0.0, 1.0 }, r);

		glm::vec3 pos = camera->GetPosition();
		pos.z = -camera->GetZoom();

		BufferData bd = { size, camera->GetWidth(), camera->GetHeight(), 0,
							glm::vec4(camera->GetPosition(), 0),
							glm::vec4(right, 0),
							glm::vec4(up, 0),
							glm::vec4(forward, 0),
							glm::vec4(center, 0) };

		/*std::cout << "#-------------------------------------------------------------------------------------------#" << std::endl;
		std::cout << voxels[0].type << " / " << voxels[0].children << std::endl;
		std::cout << "#-------------------------------------------------------------------------------------------#" << std::endl;*/

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(BufferData), &bd);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	/* WriteBuffer --------------------------------------*/
	/*
		WriteBuffer writes the current voxel data to the
		SSBO on the GPU.
	*/
	void Octree::WriteBuffer()
	{
		Quaternion r = camera->GetRotation();
		glm::vec3 right = Rotate({ 1.0, 0.0,0.0 }, r);
		glm::vec3 up = Rotate({ 0.0, 1.0,0.0 }, r);
		glm::vec3 forward = Rotate({ 0.0, 0.0, 1.0 }, r);

		glm::vec3 pos = camera->GetPosition();
		pos.z = -camera->GetZoom();

		BufferData bd = { size, camera->GetWidth(), camera->GetHeight(), 0,
							glm::vec4(camera->GetPosition(), 0),
							glm::vec4(right, 0),
							glm::vec4(up, 0),
							glm::vec4(forward, 0),
							glm::vec4(center, 0) };

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(BufferData), &bd);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(BufferData), nVoxels * sizeof(Voxel), voxels);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	/*---------------------------------------------------*/
	/* Voxel Functions								     */
	/*---------------------------------------------------*/
	/* Add Voxel ----------------------------------------*/
	/*
		AddVoxel adds a voxel given global x, y, and z
		coordinates and a type.

		Input: (Global) Coordinates & Type
		Output: None
	*/
	void Octree::AddVoxel(unsigned int x, unsigned int y, unsigned int z, uint16_t t)
	{
		/*
			We should check to see if the voxel requested
			lies outside the current window, in which case
			we'll need to set it some other way.
		*/
		// ...
		// ...
		// ...

		// Tell the system we're updating the octree.
		updated = true;

		/*
			First, we need to traverse down our voxel tree
			and add any missing non-leaf voxels in higher
			tiers.
		*/
		Voxel* target = &voxels[0];
		int s = size;
		glm::vec3 c = center;

		while (true)
		{
			/*
				Since we know the relative positions of each
				octant; it is a simple matter of A) checking
				if the current target has children and either
				B) adding the children and setting the correct
				one as the next target or C) just jumping to
				the next target.
			*/
			unsigned int octant = Diff2Oct({ x - c.x, y - c.y, z - c.z });

			// This means we need to add children.
			if (target->children < 0)
			{
				target->children = cursor;
				cursor += 8;
			}

			/*
				Now we grab the next target
				(which may be our final true
				target, if s == 1).
			*/
			target = &voxels[target->children + octant];

			/*
				If our current size is 1 then we just need
				to set the child of the current target to
				the correct type and break.
			*/
			if (s == 1)
			{
				target->type = t;
				break;
			}

			/*
				Now, we need to update the position of
				our oct centerpoint. This 
			*/
			glm::vec3 o = offsets[octant] * (float)s;
			c += o;
			s /= 2;
		}
	}

	/* RemoveVoxel --------------------------------------*/
	/*
		RemoveVoxel removes a voxel and prunes any
		resultiny empty branches. This is significantly
		more complex than adding a voxel as it requires
		us to shift memory around to avoid holes in the
		voxel array.

		Input: (Global) Coordinates
		Output: None
	*/
	void Octree::RemoveVoxel(unsigned int x, unsigned int y, unsigned int z)
	{
		/*
			We should check to see if the voxel requested
			lies outside the current window, in which case
			we'll need to set it some other way.
		*/
		// ...
		// ...
		// ...

		// Tell the system we're updating the octree.
		updated = true;

		/*
			First, we need to grab all the voxels
			in the branch corresponding to the target.
			Then, we will remove the target and work
			iteratively back up the branch pruning
			other empty voxels. This first half is
			much like the voxel-adding function.
		*/
		Voxel* target = &voxels[0];
		std::vector<Voxel*> branch = { target };
		int s = size;
		glm::vec3 c = center;

		while (true)
		{
			/*
				Since we know the relative positions of each
				octant; it is a simple matter of A) checking
				if the current target has children and either
				B) adding the children and setting the correct
				one as the next target or C) just jumping to
				the next target.
			*/
			unsigned int octant = Diff2Oct({ x - c.x, y - c.y, z - c.z });

			// This means we need to add children.
			if (target->children < 0)
			{
				target->children = cursor;
				for (int i = 0; i < 8; i++)
				{
					voxels[cursor] = { 0, -1 };
					cursor++;
				}
			}

			/*
				Now we grab the next target
				(which may be our final true
				target, if s == 1).
			*/
			target = &voxels[target->children + octant];
			branch.push_back(target);

			/*
				If our current size is 1 then we can break.
			*/
			if (s == 1) break;

			/*
				Now, we need to update the position of
				our oct centerpoint. This
			*/
			glm::vec3 o = offsets[octant] * (float)s;
			c += 0;
			s /= 2;
		}

		/*
			Now that we have our branch, we can work from
			the end backward. For each node, we check if
			it has any active children and then break
			if so. Otherwise, we prune the branch and its
			children and shift the whole array back to
			write over the resulting gap.
		*/
		for (int i = branch.size() - 1; i >= 0; i--)
		{
			target = branch[i];
			bool hasChildren = false;

			for (int j = 0; j < 8; j++)
			{
				if (voxels[target->children + j].type != 0)
				{
					hasChildren = true;
					break;
				}
			}

			/*
				If there are still active children,
				we're done.
			*/
			if (hasChildren) break;

			/*
				If we're here, that means we can prune
				this branch.
			*/
			memcpy(voxels + target->children, voxels + target->children + 8, ((double)nVoxels - (double)target->children) * sizeof(Voxel));
			cursor -= 8;
			memset(voxels + cursor, 0, ((double)cursor - (double)target->children) * sizeof(Voxel));
		}
	}

	/* CountTypedVoxels ---------------------------------*/
	/*
		Counts the number of voxels whose types aren't 0.
	*/
	unsigned int Octree::CountTypedVoxels()
	{
		unsigned int n = 0;
		for (int i = 1; i < nVoxels; i++)
		{
			if (voxels[i].type > 0) n++;
		}
		return n;
	}

	/*---------------------------------------------------*/
	/* Constructor										 */
	/*---------------------------------------------------*/
	/*
		This function takes world data from the disk and
		generates a sparse voxel octree representing it.

		For now, it just randomly inserts voxels of
		random colors so that we can test rendering.

		Input:		Size of loaded area (width / height / depth).
		Output:		None
	*/
	Octree::Octree(unsigned int size, Camera* camera)
	{
		srand(time(NULL));

		// First, we set up some preliminary variables.
		this->camera = camera;

		this->changed = false;
		this->updated = false;
		this->size = size;
		this->nVoxels = 0;
		this->nLayers = 1 + log2(size);
		this->cursor = 1;
		this->ssbo = 0;

		double h = (size - 0.5) / 2.0;
		this->center = { h, h, h };

		// Now we figure out the maximum number of voxels
		// we might have.
		for (int i = 1; i <= size; i *= 2)
		{
			nVoxels += (i * i * i);
		}

		// And then we allocate the necessary space.
		voxels = (Voxel*)malloc(nVoxels * sizeof(Voxel));

		// We'll check to see malloc worked fine.
		if (voxels == NULL) return;

		for (int i = 0; i < nVoxels; i++) voxels[i] = { 0, -1 };

		// And for utility purposes we're gonna store
		// some values for later.
		offsets =
		{
			{ -0.25, -0.25, -0.25 },
			{ -0.25, -0.25, 0.25 },
			{ -0.25, 0.25, -0.25 },
			{ -0.25, 0.25, 0.25 },
			{ 0.25, -0.25, -0.25 },
			{ 0.25, -0.25, 0.25 },
			{ 0.25, 0.25, -0.25 },
			{ 0.25, 0.25, 0.25 },
		};

		/*---------------------------------------------------*/
		/* TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY */
		/*---------------------------------------------------*/
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int z = 0; z < size; z++)
				{
					int r = rand() % 100 + 1;

					if (r > 50)
					{
						AddVoxel(x, y, z, 1);
					}
				}
			}
		}
		/*---------------------------------------------------*/
		/* TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY */
		/*---------------------------------------------------*/

		// And finally, we should set up our buffer.
		ssbo = 0;
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BufferData) + (nVoxels * sizeof(Voxel)), nullptr, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	/*---------------------------------------------------*/
	/* Deconstructor									 */
	/*---------------------------------------------------*/
	/*
		This function frees up the space allocated to the
		octree.

		Input:		None
		Output:		None
	*/
	Octree::~Octree()
	{
		glDeleteBuffers(1, &ssbo);
		free(voxels);
	}
}