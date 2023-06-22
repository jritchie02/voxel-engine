#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk()
{
}

Chunk::Chunk(const siv::PerlinNoise perlin, int xOffset, int zOffset)
{
    // Initialize the m_Blocks vector
    m_Blocks.resize(CHUNK_SIZE, std::vector<std::vector<Block>>(CHUNK_SIZE, std::vector<Block>(CHUNK_SIZE)));
    // Iterate over x, y, z coordinates to initialize each block
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                // Generate terrain using Perlin noise with offsets
                const double noise = perlin.noise3D_01(
                    (y * 0.01),
                    (x * 0.01) + (xOffset * CHUNK_SIZE * .01),
                    (z * 0.01) + (zOffset * CHUNK_SIZE * .01));

                // Define the threshold value for terrain generation
                double threshold = 0.1; // Adjust this value to control the terrain height

                // Create and assign a new block
                Block block(x, y, z);
                m_Blocks[x][y][z] = block;

                if (noise < threshold)
                {
                    m_Blocks[x][y][z].SetActive(false);
                }
            }
        }
    }

    m_frontNeighbor = nullptr;
    m_rightNeighbor = nullptr;
    m_backNeighbor = nullptr;
    m_leftNeighbor = nullptr;
}

Chunk::~Chunk()
{
}

void Chunk::setFrontNeighbor(Chunk *chunk)
{
    m_frontNeighbor = chunk;
}
void Chunk::setBackNeighbor(Chunk *chunk)
{
    m_backNeighbor = chunk;
}
void Chunk::setLeftNeighbor(Chunk *chunk)
{
    m_leftNeighbor = chunk;
}
void Chunk::setRightNeighbor(Chunk *chunk)
{
    m_rightNeighbor = chunk;
}

const std::vector<GLfloat> Chunk::get_vertex_data(int xOffset, int zOffset, std::vector<GLuint> &indices, GLuint &baseIndex)
{
    std::vector<GLfloat> vertices;

    xOffset = xOffset * CHUNK_SIZE * BLOCK_SIZE;
    zOffset = zOffset * CHUNK_SIZE * BLOCK_SIZE;

    // Iterate over the blocks in the chunk
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                Block currBlock = m_Blocks[x][y][z];
                if (currBlock.IsActive())
                {
                    std::vector<GLfloat> cubeVertices = generateCubeVertices(x, y, z, xOffset, zOffset, indices, baseIndex);
                    vertices.insert(vertices.end(), cubeVertices.begin(), cubeVertices.end());
                }
            }
        }
    }

    return vertices;
}

std::vector<GLfloat> Chunk::generateCubeVertices(int x, int y, int z, int xOffset, int zOffset, std::vector<GLuint> &indices, GLuint &baseIndex)
{
    // Calculate the half size of the cube
    float halfSize = BLOCK_SIZE * 0.5f;

    glm::vec3 blockPosition((x * BLOCK_SIZE) + xOffset, y * BLOCK_SIZE, (z * BLOCK_SIZE) + zOffset);

    // Define the color for the block based on the block type
    std::vector<GLfloat> color = {0.0f, 1.0f, 0.0f}; // Green
    std::vector<GLfloat> colorFront = {1.0f, 0.0f, 0.0f};
    std::vector<GLfloat> colorRight = {0.0f, 0.0f, 1.0f};
    std::vector<GLfloat> colorLeft = {1.0f, 0.0f, 1.0f};
    std::vector<GLfloat> colorBack = {0.4f, 0.4f, 0.4f};
    // Calculate the coordinates of the cube's vertices based on the block position
    std::vector<GLfloat> vertices;

    // Front face
    if (!hasNeighborOnFace(x, y, z, 0, 0, 1))
    {
        // std::cout << "Adding front face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, colorFront[0], colorFront[1], colorFront[2], // Vertex 0
                                            blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, colorFront[0], colorFront[1], colorFront[2], // Vertex 1
                                            blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, colorFront[0], colorFront[1], colorFront[2], // Vertex 2
                                            blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, colorFront[0], colorFront[1], colorFront[2]  // Vertex 3
                                        });
        addFace(indices, baseIndex);
    }

    // Back face
    if (!hasNeighborOnFace(x, y, z, 0, 0, -1))
    {
        // std::cout << "Adding back face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, colorBack[0], colorBack[1], colorBack[2], // Vertex 4
                                            blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, colorBack[0], colorBack[1], colorBack[2], // Vertex 5
                                            blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, colorBack[0], colorBack[1], colorBack[2], // Vertex 6
                                            blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, colorBack[0], colorBack[1], colorBack[2]  // Vertex 7
                                        });
        addFace(indices, baseIndex);
    }

    // Left face
    if (!hasNeighborOnFace(x, y, z, -1, 0, 0))
    {
        // std::cout << "Adding left face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, colorLeft[0], colorLeft[1], colorLeft[2],  // Vertex 8
                                            blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, colorLeft[0], colorLeft[1], colorLeft[2],  // Vertex 9
                                            blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, colorLeft[0], colorLeft[1], colorLeft[2], // Vertex 10
                                            blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, colorLeft[0], colorLeft[1], colorLeft[2]  // Vertex 11
                                        });
        addFace(indices, baseIndex);
    }

    // Right face
    if (!hasNeighborOnFace(x, y, z, 1, 0, 0))
    {
        // std::cout << "Adding right face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, colorRight[0], colorRight[1], colorRight[2], // Vertex 12
                                            blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, colorRight[0], colorRight[1], colorRight[2], // Vertex 13
                                            blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, colorRight[0], colorRight[1], colorRight[2], // Vertex 14
                                            blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, colorRight[0], colorRight[1], colorRight[2]  // Vertex 15
                                        });
        addFace(indices, baseIndex);
    }

    // Top face
    if (!hasNeighborOnFace(x, y, z, 0, 1, 0))
    {
        // std::cout << "Adding top face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2], // Vertex 16
                                            blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z - halfSize, color[0], color[1], color[2], // Vertex 17
                                            blockPosition.x + halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2], // Vertex 18
                                            blockPosition.x - halfSize, blockPosition.y + halfSize, blockPosition.z + halfSize, color[0], color[1], color[2]  // Vertex 19
                                        });
        addFace(indices, baseIndex);
    }

    // Bottom face
    if (!hasNeighborOnFace(x, y, z, 0, -1, 0))
    {
        // std::cout << "Adding bottom face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2], // Vertex 20
                                            blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z + halfSize, color[0], color[1], color[2], // Vertex 21
                                            blockPosition.x + halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2], // Vertex 22
                                            blockPosition.x - halfSize, blockPosition.y - halfSize, blockPosition.z - halfSize, color[0], color[1], color[2]  // Vertex 23
                                        });
        addFace(indices, baseIndex);
    }

    return vertices;
}

void Chunk::addFace(std::vector<GLuint> &indices, GLuint &baseIndex)
{
    indices.insert(indices.end(), {
                                      baseIndex, baseIndex + 1, baseIndex + 2, // Triangle 1
                                      baseIndex + 2, baseIndex + 3, baseIndex  // Triangle 2
                                  });
    baseIndex += 4;
}

bool Chunk::hasNeighborOnFace(int x, int y, int z, int offsetX, int offsetY, int offsetZ)
{
    const bool isDefault = false;

    const int neighborX = x + offsetX;
    const int neighborY = y + offsetY;
    const int neighborZ = z + offsetZ;

    if (neighborX >= 0 && neighborX < CHUNK_SIZE &&
        neighborY >= 0 && neighborY < CHUNK_SIZE &&
        neighborZ >= 0 && neighborZ < CHUNK_SIZE)
    {
        return m_Blocks[neighborX][neighborY][neighborZ].IsActive();
    }

    // Check neighboring chunks if the position is on an edge case
    if (neighborX < 0 && m_rightNeighbor != nullptr)
    {
        return m_rightNeighbor->getBlock(CHUNK_SIZE - 1, neighborY, neighborZ)->IsActive();
    }

    else if (neighborX >= CHUNK_SIZE && m_leftNeighbor != nullptr)
    {
        return m_leftNeighbor->getBlock(0, neighborY, neighborZ)->IsActive();
    }

    if (neighborZ < 0 && m_backNeighbor != nullptr)
    {
        return m_backNeighbor->getBlock(neighborX, neighborY, CHUNK_SIZE - 1)->IsActive();
    }
    else if (neighborZ >= CHUNK_SIZE && m_frontNeighbor != nullptr)
    {
        return m_frontNeighbor->getBlock(neighborX, neighborY, 0)->IsActive();
    }

    return isDefault;
}

Block *Chunk::getBlock(int x, int y, int z)
{
    return &m_Blocks[x][y][z];
}