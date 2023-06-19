#ifndef CHUNK_HPP // include guard
#define CHUNK_HPP

#include <vector>
#include <glad/glad.h>
#include "Block.hpp"

class Chunk
{
public:
    Chunk();
    ~Chunk();
    // void Update(float dt);
    // void Render(OpenGLRenderer *pRenderer);
    static const int CHUNK_SIZE = 2;
    static const int BLOCK_SIZE = 1;
    const std::vector<GLfloat> get_vertex_data();
    const std::vector<GLuint> get_index_data();

   

private: // The blocks data
    std::vector<std::vector<std::vector<Block>>> m_Blocks;
    std::vector<GLfloat> generateCubeVertices(glm::vec3 &blockPosition);
    std::vector<GLuint> generateBlockIndices(GLuint baseIndex);
};

#endif /* CHUNK_HPP */