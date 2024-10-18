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
    const std::vector<GLfloat> GetVertexData(int xOffset, int zOffset, std::vector<GLuint> &indices, GLuint &baseIndex);
    Block *GetBlock(int x, int y, int z);
    void UpdateBlock(int x, int y, int z, bool isActive);
    bool IsInBounds(float x, float y, float z);

    // Setters
    void SetFrontNeighbor(Chunk *chunk);
    void SetBackNeighbor(Chunk *chunk);
    void SetLeftNeighbor(Chunk *chunk);
    void SetRightNeighbor(Chunk *chunk);

private: 
    // Methods
    std::vector<GLfloat> GenerateCubeVertices(int x, int y, int z, std::vector<GLuint> &indices, GLuint &baseIndex);
    bool HasNeighborOnFace(int x, int y, int z, int offsetX, int offsetY, int offsetZ);
    void AddFace(std::vector<GLuint> &indices, GLuint &baseIndex);

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