#ifndef CHUNK_HPP // include guard
#define CHUNK_HPP

#include <vector>
#include <glad/glad.h>
#include "Block.hpp"
#include "PerlinNoise.hpp"

class Chunk
{
public:
    // Constructors/Destructor
    Chunk();
    Chunk(const siv::PerlinNoise perlin, int xOffset, int zOffset);
    ~Chunk();

    // Constants
    static const int CHUNK_SIZE = 16;
    static constexpr float BLOCK_SIZE = 1.0f;

    // Methods
    const std::vector<GLfloat> get_vertex_data(int xOffset, int zOffset, std::vector<GLuint> &indices, GLuint &baseIndex);
    Block *getBlock(int x, int y, int z);
    void updateBlock(int x, int y, int z, bool isActive);
    bool isInBounds(float x, float y, float z);

    // Setters
    void setFrontNeighbor(Chunk *chunk);
    void setBackNeighbor(Chunk *chunk);
    void setLeftNeighbor(Chunk *chunk);
    void setRightNeighbor(Chunk *chunk);

private: 
    // Methods
    std::vector<GLfloat> generateCubeVertices(int x, int y, int z, std::vector<GLuint> &indices, GLuint &baseIndex);
    bool hasNeighborOnFace(int x, int y, int z, int offsetX, int offsetY, int offsetZ);
    void addFace(std::vector<GLuint> &indices, GLuint &baseIndex);

    // Member Variables
    std::vector<std::vector<std::vector<Block>>> m_Blocks;
    int m_xOffset;
    int m_zOffset;
    Chunk *m_frontNeighbor;
    Chunk *m_backNeighbor;
    Chunk *m_leftNeighbor;
    Chunk *m_rightNeighbor;
};

#endif /* CHUNK_HPP */