## Voxel Landscape Renderer 

Welcome to my Voxel renderer. Here is a single voxel. A simple 6 sided cube composed of traingles. A single voxel stores its type and knows if it is active or disabled. 
![SingleCube](https://github.com/Summer23Graphics/finalproject-jared-ritchie/assets/91036922/53cbe63c-cb30-4ec5-8daf-065b34fde0e0)

Voxels are like blocks that are stored in Chunks. Chunks hold a 3D array of Blocks that help keep track of their position in the world. Here is an example of a single Chunk. This chunk has 4 voxels in its width/height/length
![1Chunk4Cubes](https://github.com/Summer23Graphics/finalproject-jared-ritchie/assets/91036922/dd012b4c-b34a-426a-be07-86115aa51367)

Chunks are stored in a Chunk Manager. This stores a 2d grid of Chunks. Here is a grid that has 4 Chunks that each have 4 Voxels. As you can see the geometry quickly adds up. In a naive voxel renderer every voxel renders all 6 faces. In wireframe mode this is useful although in standard shading mode we are rendering triangles that cannot be seen. 
![2Chunk2Cubes](https://github.com/Summer23Graphics/finalproject-jared-ritchie/assets/91036922/66b3feb5-5b46-4561-9ea8-35df73ef9468)

This is a single chunk that accounts for unseen voxels. If a voxel is not on the outside of a chunk, its geometry is not drawn. This signifcantly reduces the number of vertices drawn since all singe traingles are no longer rendered. 
![2Chunk2CubesHollow](https://github.com/Summer23Graphics/finalproject-jared-ritchie/assets/91036922/052011c7-1bdb-4163-a373-f268a4212f0b)

Next, the Chunk Manager needs away to determine if voxels should be drawn that neighbor other chunks. In this case we can pass a pointer each chunk that references its neighboring chunks on each side. Then we can determine if the edge voxels should be drawn. This is how multiple chunks look rendered next to eachother in a bigger scale. Now, we do not always render voxels that are on the outside border of a chunk.
![LargeWireframe](https://github.com/Summer23Graphics/finalproject-jared-ritchie/assets/91036922/ea91ce72-ac1c-4414-b977-2faef102a485)

The photo below displays the huge number of uneeded faces that are rendered if neighboring chunks are not compared. 
![LargeWireframeBorders](https://github.com/Summer23Graphics/finalproject-jared-ritchie/assets/91036922/8503a5cd-0520-4cd0-8600-7365567ac34f)

Terrain is handled by using a Perlin Noise generator. Each chunk is passed an offset so continious noise can be calculated creating a seamless environment between chunks. The perlin noise generator used in this project can be found here https://github.com/Reputeless/PerlinNoise

The two photos above are rendering a 4x4 grid of Chunks that are 16/16/16 voxels large. Prior to boundary checking the program needs to render
  * 405744 vertices and 101436 indices

After boundary checking we reduce the number to
  * 218832 vertices and 54708 indices

This project is largely inspired by Minecrafts terrain generation system. To expand this project, an algorithm like Greedy meshing can be applied to collapse triangle faces on the same plane into larger sections. This would reduce the over vertex count drastically.

To run the program run 
  * python3 build.py
  * ./project.exe
  * WASD to move camera and Mouse scroll to move up and down
  * To modify the number of chunks, or voxels per chunk update the CHUNK_SIZE/CHUNK_GRID_SIZE in Chunk.hpp and ChunkManager.hpp and recompile.


External Sources
  * https://sites.google.com/site/letsmakeavoxelengine/home/basic-block-rendering?authuser=0 This website/blog was also used to help me understand some of the voxel engine concepts at high level. The header files for Chunk and Block representations were inspired from this source
