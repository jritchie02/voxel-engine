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
    Chunk(const siv::PerlinNoise perlin);
    ~Chunk();
    // void Update(float dt);
    static const int CHUNK_SIZE = 16;
    static constexpr float BLOCK_SIZE = 0.5f;
    const std::vector<GLfloat> get_vertex_data();
    const std::vector<GLuint> get_index_data();
    
private: // The blocks data
    std::vector<std::vector<std::vector<Block>>> m_Blocks;
    std::vector<GLfloat> generateCubeVertices(int x, int y, int z);
    std::vector<GLuint> generateBlockIndices(GLuint &baseIndex, int x, int y, int z);
    bool hasNeighborOnFace(int x, int y, int z, int offsetX, int offsetY, int offsetZ);
    void addFace(std::vector<GLuint> &indices, GLuint &baseIndex);
};

#endif /* CHUNK_HPP */