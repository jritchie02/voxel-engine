#ifndef CHUNK_HPP // include guard
#define CHUNK_HPP

#include <vector>
#include <glad/glad.h>
#include "Block.hpp"
#include "PerlinNoise.hpp"

class Chunk
{
public:
    Chunk();
    Chunk(const siv::PerlinNoise perlin, int xOffset, int zOffset);
    ~Chunk();
    // void Update(float dt);
    static const int CHUNK_SIZE = 16;
    static constexpr float BLOCK_SIZE = 0.5f;
    const std::vector<GLfloat> get_vertex_data(int xOffset, int zOffset);
    const std::vector<GLuint> get_index_data(GLuint &baseIndex);

    void setFrontNeighbor(Chunk *chunk);
    void setBackNeighbor(Chunk *chunk);
    void setLeftNeighbor(Chunk *chunk);
    void setRightNeighbor(Chunk *chunk);
    Block* getBlock(int x, int y, int z);

private: // The blocks data
    std::vector<std::vector<std::vector<Block>>> m_Blocks;
    std::vector<GLfloat> generateCubeVertices(int x, int y, int z, int xOffset, int zOffset);
    std::vector<GLuint> generateBlockIndices(GLuint &baseIndex, int x, int y, int z);
    bool hasNeighborOnFace(int x, int y, int z, int offsetX, int offsetY, int offsetZ);
    void addFace(std::vector<GLuint> &indices, GLuint &baseIndex);
    void collapseTriangles(std::vector<GLfloat> &vertices, int idx1, int idx2, int idx3, int idx4, int idx5, int idx6);

    Chunk *m_frontNeighbor;
    Chunk *m_backNeighbor;
    Chunk *m_leftNeighbor;
    Chunk *m_rightNeighbor;
};

#endif /* CHUNK_HPP */