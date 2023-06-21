#ifndef CHUNKMANAGER_HPP 
#define CHUNKMANAGER_HPP

#include <vector>
#include "Chunk.hpp"
#include "PerlinNoise.hpp"

class ChunkManager
{
public:
    static const int CHUNK_GRID_SIZE = 4; // Number of chunks in the grid

    ChunkManager();
    ~ChunkManager();

    void GenerateChunks();
    //void RenderChunks();
    void UpdateChunks(float dt);
    const std::vector<GLfloat> get_vertex_data();
    const std::vector<GLuint> get_index_data();

private:
    // 2D grid of chunks
    std::vector<std::vector<Chunk*>> m_ChunkGrid;

};

#endif /* CHUNKMANAGER_HPP */