#include "Voxel.hpp"
#include <iostream>

Voxel::Voxel(glm::vec3 position, float width, glm::vec2 texture_position)
{
    m_position = position;
    m_width = width;
    m_texture_position = texture_position;
}

glm::vec3 Voxel::GetPosition()
{
    return m_position;
}

glm::vec4 Voxel::GetTextureCoordinates(int x, int y)
{
    const int PPB = 16; // Pixels per block
    const float BLOCK_SIZE = 1.0f / 16;
    const float u0 = x * BLOCK_SIZE; // Bottom-left corner
    const float v0 = y * BLOCK_SIZE;
    const float u1 = x * BLOCK_SIZE + BLOCK_SIZE; // Top-right corner
    const float v1 = y * BLOCK_SIZE + BLOCK_SIZE;

    return glm::vec4(u0, v0, u1, v1);
}

std::vector<GLfloat> Voxel::GetVertexData()
{

    std::vector<GLfloat> vertices;

    // Get the texture coordinates
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Front face
                                        m_position.x, m_position.y + m_width, m_position.z + m_width, u0, v1, 0.0f, 0.0f, 1.0f,           // Vertex 0
                                        m_position.x + m_width, m_position.y + m_width, m_position.z + m_width, u1, v1, 0.0f, 0.0f, 1.0f, // Vertex 1
                                        m_position.x + m_width, m_position.y, m_position.z + m_width, u1, v0, 0.0f, 0.0f, 1.0f,           // Vertex 2
                                        m_position.x, m_position.y, m_position.z + m_width, u0, v0, 0.0f, 0.0f, 1.0f,                     // Vertex 3
                                    });

    // Back face
    vertices.insert(vertices.end(), {
                                        m_position.x + m_width, m_position.y + m_width, m_position.z, u0, v1, 0.0f, 0.0f, -1.0f, // Vertex 4
                                        m_position.x, m_position.y + m_width, m_position.z, u1, v1, 0.0f, 0.0f, -1.0f,           // Vertex 5
                                        m_position.x, m_position.y, m_position.z, u1, v0, 0.0f, 0.0f, -1.0f,                     // Vertex 6
                                        m_position.x + m_width, m_position.y, m_position.z, u0, v0, 0.0f, 0.0f, -1.0f,           // Vertex 7
                                    });

    // Left face (normal: -1, 0, 0)
    vertices.insert(vertices.end(), {
                                        m_position.x, m_position.y + m_width, m_position.z, u0, v1, -1.0f, 0.0f, 0.0f,           // Vertex 8
                                        m_position.x, m_position.y + m_width, m_position.z + m_width, u1, v1, -1.0f, 0.0f, 0.0f, // Vertex 9
                                        m_position.x, m_position.y, m_position.z + m_width, u1, v0, -1.0f, 0.0f, 0.0f,           // Vertex 10
                                        m_position.x, m_position.y, m_position.z, u0, v0, -1.0f, 0.0f, 0.0f,                     // Vertex 11
                                    });

    // Right face (normal: 1, 0, 0)
    vertices.insert(vertices.end(), {
                                        m_position.x + m_width, m_position.y + m_width, m_position.z + m_width, u0, v1, 1.0f, 0.0f, 0.0f, // Vertex 12
                                        m_position.x + m_width, m_position.y + m_width, m_position.z, u1, v1, 1.0f, 0.0f, 0.0f,           // Vertex 13
                                        m_position.x + m_width, m_position.y, m_position.z, u1, v0, 1.0f, 0.0f, 0.0f,                     // Vertex 14
                                        m_position.x + m_width, m_position.y, m_position.z + m_width, u0, v0, 1.0f, 0.0f, 0.0f,           // Vertex 15
                                    });

    // Top face (normal: 0, 1, 0)
    vertices.insert(vertices.end(), {
                                        m_position.x, m_position.y + m_width, m_position.z, u0, v1, 0.0f, 1.0f, 0.0f,                     // Vertex 16
                                        m_position.x + m_width, m_position.y + m_width, m_position.z, u1, v1, 0.0f, 1.0f, 0.0f,           // Vertex 17
                                        m_position.x + m_width, m_position.y + m_width, m_position.z + m_width, u1, v0, 0.0f, 1.0f, 0.0f, // Vertex 18
                                        m_position.x, m_position.y + m_width, m_position.z + m_width, u0, v0, 0.0f, 1.0f, 0.0f,           // Vertex 19
                                    });

    // Bottom face (normal: 0, -1, 0)
    vertices.insert(vertices.end(), {
                                        m_position.x, m_position.y, m_position.z + m_width, u0, v1, 0.0f, -1.0f, 0.0f,           // Vertex 20
                                        m_position.x + m_width, m_position.y, m_position.z + m_width, u1, v1, 0.0f, -1.0f, 0.0f, // Vertex 21
                                        m_position.x + m_width, m_position.y, m_position.z, u1, v0, 0.0f, -1.0f, 0.0f,           // Vertex 22
                                        m_position.x, m_position.y, m_position.z, u0, v0, 0.0f, -1.0f, 0.0f,                     // Vertex 23
                                    });

    return vertices;
}

void Voxel::GetIndexData(std::vector<GLuint> &indices, GLuint &nextIndex)
{
    // For each 6 faces of the cube, we need to add 4 indices
    for (int i = 0; i < 6; i++)
    {
        indices.insert(indices.end(), {
                                          nextIndex, nextIndex + 1, nextIndex + 2, // Triangle 1
                                          nextIndex + 2, nextIndex + 3, nextIndex  // Triangle 2
                                      });
        nextIndex += 4;
    }
}

std::vector<GLfloat> Voxel::GetFrontVertices()
{
    std::vector<GLfloat> vertices;
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Front face
                                        m_position.x, m_position.y + m_width, m_position.z + m_width, u0, v1, 0.0f, 0.0f, 1.0f,           // Vertex 0
                                        m_position.x + m_width, m_position.y + m_width, m_position.z + m_width, u1, v1, 0.0f, 0.0f, 1.0f, // Vertex 1
                                        m_position.x + m_width, m_position.y, m_position.z + m_width, u1, v0, 0.0f, 0.0f, 1.0f,           // Vertex 2
                                        m_position.x, m_position.y, m_position.z + m_width, u0, v0, 0.0f, 0.0f, 1.0f,                     // Vertex 3
                                    });

    return vertices;
}

std::vector<GLfloat> Voxel::GetBackVertices()
{
    std::vector<GLfloat> vertices;
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Back face
                                        m_position.x + m_width, m_position.y + m_width, m_position.z, u0, v1, 0.0f, 0.0f, -1.0f, // Vertex 4
                                        m_position.x, m_position.y + m_width, m_position.z, u1, v1, 0.0f, 0.0f, -1.0f,           // Vertex 5
                                        m_position.x, m_position.y, m_position.z, u1, v0, 0.0f, 0.0f, -1.0f,                     // Vertex 6
                                        m_position.x + m_width, m_position.y, m_position.z, u0, v0, 0.0f, 0.0f, -1.0f,           // Vertex 7
                                    });

    return vertices;
}

std::vector<GLfloat> Voxel::GetLeftVertices()
{
    std::vector<GLfloat> vertices;
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Left face
                                        m_position.x, m_position.y + m_width, m_position.z, u0, v1, -1.0f, 0.0f, 0.0f,           // Vertex 8
                                        m_position.x, m_position.y + m_width, m_position.z + m_width, u1, v1, -1.0f, 0.0f, 0.0f, // Vertex 9
                                        m_position.x, m_position.y, m_position.z + m_width, u1, v0, -1.0f, 0.0f, 0.0f,           // Vertex 10
                                        m_position.x, m_position.y, m_position.z, u0, v0, -1.0f, 0.0f, 0.0f,                     // Vertex 11
                                    });

    return vertices;
}

std::vector<GLfloat> Voxel::GetRightVertices()
{
    std::vector<GLfloat> vertices;
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Right face
                                        m_position.x + m_width, m_position.y + m_width, m_position.z + m_width, u0, v1, 1.0f, 0.0f, 0.0f, // Vertex 12
                                        m_position.x + m_width, m_position.y + m_width, m_position.z, u1, v1, 1.0f, 0.0f, 0.0f,           // Vertex 13
                                        m_position.x + m_width, m_position.y, m_position.z, u1, v0, 1.0f, 0.0f, 0.0f,                     // Vertex 14
                                        m_position.x + m_width, m_position.y, m_position.z + m_width, u0, v0, 1.0f, 0.0f, 0.0f,           // Vertex 15
                                    });

    return vertices;
}

std::vector<GLfloat> Voxel::GetTopVertices()
{
    std::vector<GLfloat> vertices;
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Top face
                                        m_position.x, m_position.y + m_width, m_position.z, u0, v1, 0.0f, 1.0f, 0.0f,                     // Vertex 16
                                        m_position.x + m_width, m_position.y + m_width, m_position.z, u1, v1, 0.0f, 1.0f, 0.0f,           // Vertex 17
                                        m_position.x + m_width, m_position.y + m_width, m_position.z + m_width, u1, v0, 0.0f, 1.0f, 0.0f, // Vertex 18
                                        m_position.x, m_position.y + m_width, m_position.z + m_width, u0, v0, 0.0f, 1.0f, 0.0f,           // Vertex 19
                                    });

    return vertices;
}

std::vector<GLfloat> Voxel::GetBottomVertices()
{
    std::vector<GLfloat> vertices;
    glm::vec4 textureCoordinates = GetTextureCoordinates(m_texture_position.x, m_texture_position.y);
    float u0 = textureCoordinates.x;
    float v0 = textureCoordinates.y;
    float u1 = textureCoordinates.z;
    float v1 = textureCoordinates.w;

    vertices.insert(vertices.end(), {
                                        // Bottom face
                                        m_position.x, m_position.y, m_position.z + m_width, u0, v1, 0.0f, -1.0f, 0.0f,           // Vertex 20
                                        m_position.x + m_width, m_position.y, m_position.z + m_width, u1, v1, 0.0f, -1.0f, 0.0f, // Vertex 21
                                        m_position.x + m_width, m_position.y, m_position.z, u1, v0, 0.0f, -1.0f, 0.0f,           // Vertex 22
                                        m_position.x, m_position.y, m_position.z, u0, v0, 0.0f, -1.0f, 0.0f,                     // Vertex 23
                                    });

    return vertices;
}