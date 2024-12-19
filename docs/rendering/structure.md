# Structure of the Rendering System
## Overview
 The rendering system of *Winedark* is built to handle potentially millions of voxels at a given
 time. For a long time now, I have wanted to a implement a sparse voxel octree system.
 Hypothetically, we could represent each game-level voxel as a smaller set of single-color voxels,
 thereby bypassing the need for textures. Furthermore, this would allow us to play around with nice
 rendering techniques such as volumetric ray tracing. However, this requires that I dig into the
 current literature on efficient voxel octree traversal algorithms.

 The most obvious place to begin is [this paper](https://www.nvidia.com/docs/IO/88972/nvr-2010-001.pdf)
 by Samuli Laine and Tero Karras at Nvidia.
