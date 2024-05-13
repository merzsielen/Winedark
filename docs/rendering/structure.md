# Structure of the Rendering System
## Overview
 The rendering system of *Winedark* is built to handle potentially millions of voxels at a given
 time. To achieve this, it uses a somewhat arcane system which compresses its voxels into an
 octree structure and passes this to the shader which then uses a traversal algorithm to render the
 proper voxels to the screen.