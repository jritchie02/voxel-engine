#include "Block.hpp"

Block::Block()
{
    m_vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f, // Vertex 0
         0.5f, -0.5f,  0.5f, // Vertex 1
         0.5f,  0.5f,  0.5f, // Vertex 2
        -0.5f,  0.5f,  0.5f, // Vertex 3

        // Back face
        -0.5f, -0.5f, -0.5f, // Vertex 4
         0.5f, -0.5f, -0.5f, // Vertex 5
         0.5f,  0.5f, -0.5f, // Vertex 6
        -0.5f,  0.5f, -0.5f, // Vertex 7

        // Left face
        -0.5f, -0.5f, -0.5f, // Vertex 8
        -0.5f, -0.5f,  0.5f, // Vertex 9
        -0.5f,  0.5f,  0.5f, // Vertex 10
        -0.5f,  0.5f, -0.5f, // Vertex 11

        // Right face
         0.5f, -0.5f, -0.5f, // Vertex 12
         0.5f, -0.5f,  0.5f, // Vertex 13
         0.5f,  0.5f,  0.5f, // Vertex 14
         0.5f,  0.5f, -0.5f, // Vertex 15

        // Top face
        -0.5f,  0.5f,  0.5f, // Vertex 16
         0.5f,  0.5f,  0.5f, // Vertex 17
         0.5f,  0.5f, -0.5f, // Vertex 18
        -0.5f,  0.5f, -0.5f, // Vertex 19

        // Bottom face
        -0.5f, -0.5f, -0.5f, // Vertex 20
         0.5f, -0.5f, -0.5f, // Vertex 21
         0.5f, -0.5f,  0.5f, // Vertex 22
        -0.5f, -0.5f,  0.5f  // Vertex 23
    };

    m_indices = {
        // Front face
        0, 1, 2, // Triangle 1
        2, 3, 0, // Triangle 2

        // Back face
        4, 5, 6, // Triangle 1
        6, 7, 4, // Triangle 2

        // Left face
        8, 9, 10, // Triangle 1
        10, 11, 8, // Triangle 2

        // Right face
        12, 13, 14, // Triangle 1
        14, 15, 12, // Triangle 2

        // Top face
        16, 17, 18, // Triangle 1
        18, 19, 16, // Triangle 2

        // Bottom face
        20, 21, 22, // Triangle 1
        22, 23, 20  // Triangle 2
    };

    m_blockType = BlockType_Grass;

}

Block::~Block()
{

}

const std::vector<GLfloat> Block::get_vertex_data()
{
    // Define the color for the block based on the block type
    std::vector<GLfloat> color;
    switch (m_blockType)
    {
        case BlockType_Grass:
            color = { 0.0f, 1.0f, 0.0f };   // Green
            break;
        case BlockType_Dirt:
            color = { 0.6f, 0.3f, 0.0f };   // Brown
            break;
        case BlockType_Water:
            color = { 0.0f, 0.0f, 1.0f };   // Blue
            break;
        case BlockType_Stone:
            color = { 0.5f, 0.5f, 0.5f };   // Gray
            break;
        case BlockType_Wood:
            color = { 0.5f, 0.35f, 0.05f }; // Dark brown
            break;
        case BlockType_Sand:
            color = { 1.0f, 0.9f, 0.0f };   // Yellow
            break;
        default:
            color = { 1.0f, 1.0f, 1.0f };   // Default color (white)
            break;
    }

    // Create the vertices with the associated color
    std::vector<GLfloat> vertices;
    for (int i = 0; i < m_vertices.size(); i += 3)
    {
        vertices.push_back(m_vertices[i]);
        vertices.push_back(m_vertices[i + 1]);
        vertices.push_back(m_vertices[i + 2]);
        vertices.push_back(color[0]);
        vertices.push_back(color[1]);
        vertices.push_back(color[2]);
    }

    return vertices;
}

const std::vector<GLuint> Block::get_index_data()
{
    return m_indices;
}
