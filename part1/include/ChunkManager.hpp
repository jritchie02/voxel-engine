#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <vector>
#include "Chunk.hpp"
#include "PerlinNoise.hpp"

class ChunkManager
{
public:
    // Constructor/ Destructor
    ChunkManager();
    ~ChunkManager();

    // Constant
    static const int CHUNK_GRID_SIZE = 8; // Y x Y grid Ex: 2 is 2x2 grid

    // Methods
    void GenerateChunks();
    void UpdateChunks(int x, int y, int z);

    const std::vector<GLfloat> get_vertex_data();
    const std::vector<GLuint> get_index_data();

private:
    // 2D grid of chunk pointers
    std::vector<std::vector<Chunk *>> m_ChunkGrid;
    // Indices of triangle faces
    std::vector<GLuint> m_indexes;
};

#endif /* CHUNKMANAGER_HPP */