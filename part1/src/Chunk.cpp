#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk()
{
    std::cout << "Chunk constructor" << std::endl;

    // Initialize the m_Blocks vector
    m_Blocks.resize(CHUNK_SIZE, std::vector<std::vector<Block>>(CHUNK_SIZE, std::vector<Block>(CHUNK_SIZE)));

    // Iterate over x, y, z coordinates to initialize each block
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                // Create and assign a new block
                Block block(x, y, z);
                m_Blocks[x][y][z] = block;
            }
        }
    }

    std::cout << "Chunk constructor end" << std::endl;
}

Chunk::~Chunk()
{
}

const std::vector<GLfloat> Chunk::get_vertex_data()
{
    std::vector<GLfloat> vertices;

    // Iterate over the blocks in the chunk
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (m_Blocks.at(x).at(y).at(z).IsActive())
                {
                    // Calculate the position of the current block in world space
                    glm::vec3 blockPosition(x * BLOCK_SIZE, y * BLOCK_SIZE, z * BLOCK_SIZE);

                    // Generate the cube vertices for the current block at the calculated position
                    std::vector<GLfloat> cubeVertices = generateCubeVertices(blockPosition);

                    // Append the cube vertices to the main vertex array
                    vertices.insert(vertices.end(), cubeVertices.begin(), cubeVertices.end());
                }
            }
        }
    }

    return vertices;
}

std::vector<GLfloat> Chunk::generateCubeVertices(glm::vec3 &blockPosition)
{
    // Calculate the half size of the cube
    float halfSize = BLOCK_SIZE * 0.5f;

    // Define the color for the block based on the block type
    std::vector<GLfloat> color = {0.0f, 1.0f, 0.0f}; // Green

    // Calculate the coordinates of the cube's vertices based on the block position
    std::vector<GLfloat> vertices = {
        // Front face
        blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 0
        blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 1
        blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 2
        blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 3

        // Back face
        blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 4
        blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 5
        blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 6
        blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 7

        // Left face
        blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 8
        blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 9
        blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 10
        blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 11

        // Right face
        blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 12
        blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 13
        blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 14
        blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 15

        // Top face
        blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 16
        blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 17
        blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 18
        blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 19

        // Bottom face
        blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 20
        blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2],// Vertex 21
        blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2],// Vertex 22
        blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize,  color[0], color[1], color[2]// Vertex 23
    };

    return vertices;
}

const std::vector<GLuint> Chunk::get_index_data()
{
    std::vector<GLuint> indices;
    GLuint baseIndex = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                Block &currBlock = m_Blocks[x][y][z];
                if (currBlock.IsActive())
                {
                    // Generate the index data for the current block
                    std::vector<GLuint> blockIndices = generateBlockIndices(baseIndex);

                    // Append the block indices to the main index array
                    indices.insert(indices.end(), blockIndices.begin(), blockIndices.end());

                    // Update the base index for the next block
                    baseIndex += 24;
                }
            }
        }
    }

    return indices;
}

std::vector<GLuint> Chunk::generateBlockIndices(GLuint baseIndex)
{
    std::vector<GLuint> indices = {
        // Front face
        baseIndex, baseIndex + 1, baseIndex + 2, // Triangle 1
        baseIndex + 2, baseIndex + 3, baseIndex, // Triangle 2

        // Back face
        baseIndex + 4, baseIndex + 5, baseIndex + 6, // Triangle 1
        baseIndex + 6, baseIndex + 7, baseIndex + 4, // Triangle 2

        // Left face
        baseIndex + 8, baseIndex + 9, baseIndex + 10,  // Triangle 1
        baseIndex + 10, baseIndex + 11, baseIndex + 8, // Triangle 2

        // Right face
        baseIndex + 12, baseIndex + 13, baseIndex + 14, // Triangle 1
        baseIndex + 14, baseIndex + 15, baseIndex + 12, // Triangle 2

        // Top face
        baseIndex + 16, baseIndex + 17, baseIndex + 18, // Triangle 1
        baseIndex + 18, baseIndex + 19, baseIndex + 16, // Triangle 2

        // Bottom face
        baseIndex + 20, baseIndex + 21, baseIndex + 22, // Triangle 1
        baseIndex + 22, baseIndex + 23, baseIndex + 20  // Triangle 2
    };

    return indices;
}