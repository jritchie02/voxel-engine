#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk()
{
    m_xOffset = 0;
    m_zOffset = 0;

    m_frontNeighbor = nullptr;
    m_rightNeighbor = nullptr;
    m_backNeighbor = nullptr;
    m_leftNeighbor = nullptr;
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
                double threshold = 0.56; // Adjust this value to control the terrain height

                // Create and assign a new block
                Block block;
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

    m_xOffset = xOffset * CHUNK_SIZE * BLOCK_SIZE;
    m_zOffset = zOffset * CHUNK_SIZE * BLOCK_SIZE;
}

Chunk::~Chunk()
{
}

void Chunk::SetFrontNeighbor(Chunk *chunk)
{
    m_frontNeighbor = chunk;
}
void Chunk::SetBackNeighbor(Chunk *chunk)
{
    m_backNeighbor = chunk;
}
void Chunk::SetLeftNeighbor(Chunk *chunk)
{
    m_leftNeighbor = chunk;
}
void Chunk::SetRightNeighbor(Chunk *chunk)
{
    m_rightNeighbor = chunk;
}

void Chunk::UpdateBlock(int x, int y, int z, bool isActive)
{
    x -= m_xOffset;
    z -= m_zOffset;

    std::cout << "Getting block at " << x << ", " << y << ", " << z << std::endl;

    if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE ||
        x < 0 || y < 0 || z < 0)
    {
        std::cout << "block is out of bounds " << std::endl;
    }
    else
    {
        m_Blocks[x][y][z].SetActive(isActive);
    }
}

const std::vector<GLfloat> Chunk::GetVertexData(int xOffset, int zOffset, std::vector<GLuint> &indices, GLuint &baseIndex)
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
                    std::vector<GLfloat> cubeVertices = GenerateCubeVertices(x, y, z, indices, baseIndex);
                    vertices.insert(vertices.end(), cubeVertices.begin(), cubeVertices.end());
                }
            }
        }
    }

    return vertices;
}

bool Chunk::IsInBounds(float x, float y, float z)
{
    bool result = (x >= m_xOffset && x < m_xOffset + (CHUNK_SIZE * BLOCK_SIZE) &&
                   z >= m_zOffset && z < m_zOffset + (CHUNK_SIZE * BLOCK_SIZE));
    int xOffsetTotal = m_xOffset + (CHUNK_SIZE * BLOCK_SIZE);
    int zOffsetTotal = m_zOffset + (CHUNK_SIZE * BLOCK_SIZE);

    return result;
}

std::vector<GLfloat> Chunk::GenerateCubeVertices(int x, int y, int z, std::vector<GLuint> &indices, GLuint &baseIndex)
{
    glm::vec3 blockPosition((x * BLOCK_SIZE) + m_xOffset, y * BLOCK_SIZE, (z * BLOCK_SIZE) + m_zOffset);

    // Define the color for the block based on the block type
    std::vector<GLfloat> color = {0.0f, 1.0f, 0.0f}; // Green
    // Calculate the coordinates of the cube's vertices based on the block position
    std::vector<GLfloat> vertices;

    // Front face
    if (!HasNeighborOnFace(x, y, z, 0, 0, 1))
    {
        // std::cout << "Adding front face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x, blockPosition.y + BLOCK_SIZE, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2],              // Vertex 0
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y + BLOCK_SIZE, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2], // Vertex 1
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2],              // Vertex 2
                                            blockPosition.x, blockPosition.y, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2]                            // Vertex 3
                                        });
        AddFace(indices, baseIndex);
    }

    // Back face
    if (!HasNeighborOnFace(x, y, z, 0, 0, -1))
    {
        // std::cout << "Adding back face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y + BLOCK_SIZE, blockPosition.z, color[0], color[1], color[2], // Vertex 4
                                            blockPosition.x, blockPosition.y + BLOCK_SIZE, blockPosition.z, color[0], color[1], color[2],              // Vertex 5
                                            blockPosition.x, blockPosition.y, blockPosition.z, color[0], color[1], color[2],                           // Vertex 6
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y, blockPosition.z, color[0], color[1], color[2]               // Vertex 7
                                        });
        AddFace(indices, baseIndex);
    }

    // Left face
    if (!HasNeighborOnFace(x, y, z, -1, 0, 0))
    {
        // std::cout << "Adding left face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x, blockPosition.y + BLOCK_SIZE, blockPosition.z, color[0], color[1], color[2],              // Vertex 8
                                            blockPosition.x, blockPosition.y + BLOCK_SIZE, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2], // Vertex 9
                                            blockPosition.x, blockPosition.y, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2],              // Vertex 10
                                            blockPosition.x, blockPosition.y, blockPosition.z, color[0], color[1], color[2]                            // Vertex 11
                                        });
        AddFace(indices, baseIndex);
    }

    // Right face
    if (!HasNeighborOnFace(x, y, z, 1, 0, 0))
    {
        // std::cout << "Adding right face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y + BLOCK_SIZE, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2], // Vertex 12
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y + BLOCK_SIZE, blockPosition.z, color[0], color[1], color[2],              // Vertex 13
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y, blockPosition.z, color[0], color[1], color[2],                           // Vertex 14
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2]               // Vertex 15
                                        });
        AddFace(indices, baseIndex);
    }

    // Top face
    if (!HasNeighborOnFace(x, y, z, 0, 1, 0))
    {
        // std::cout << "Adding top face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x, blockPosition.y + BLOCK_SIZE, blockPosition.z, color[0], color[1], color[2],                           // Vertex 16
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y + BLOCK_SIZE, blockPosition.z, color[0], color[1], color[2],              // Vertex 17
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y + BLOCK_SIZE, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2], // Vertex 18
                                            blockPosition.x, blockPosition.y + BLOCK_SIZE, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2]               // Vertex 19
                                        });
        AddFace(indices, baseIndex);
    }

    // Bottom face
    if (!HasNeighborOnFace(x, y, z, 0, -1, 0))
    {
        // std::cout << "Adding bottom face" << std::endl;
        vertices.insert(vertices.end(), {
                                            blockPosition.x, blockPosition.y, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2],              // Vertex 20
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y, blockPosition.z + BLOCK_SIZE, color[0], color[1], color[2], // Vertex 21
                                            blockPosition.x + BLOCK_SIZE, blockPosition.y, blockPosition.z, color[0], color[1], color[2],              // Vertex 22
                                            blockPosition.x, blockPosition.y, blockPosition.z, color[0], color[1], color[2]                            // Vertex 23
                                        });
        AddFace(indices, baseIndex);
    }

    return vertices;
}

void Chunk::AddFace(std::vector<GLuint> &indices, GLuint &baseIndex)
{
    indices.insert(indices.end(), {
                                      baseIndex, baseIndex + 1, baseIndex + 2, // Triangle 1
                                      baseIndex + 2, baseIndex + 3, baseIndex  // Triangle 2
                                  });
    baseIndex += 4;
}

bool Chunk::HasNeighborOnFace(int x, int y, int z, int offsetX, int offsetY, int offsetZ)
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
        return m_rightNeighbor->GetBlock(CHUNK_SIZE - 1, neighborY, neighborZ)->IsActive();
    }

    else if (neighborX >= CHUNK_SIZE && m_leftNeighbor != nullptr)
    {
        return m_leftNeighbor->GetBlock(0, neighborY, neighborZ)->IsActive();
    }

    if (neighborZ < 0 && m_backNeighbor != nullptr)
    {
        return m_backNeighbor->GetBlock(neighborX, neighborY, CHUNK_SIZE - 1)->IsActive();
    }
    else if (neighborZ >= CHUNK_SIZE && m_frontNeighbor != nullptr)
    {
        return m_frontNeighbor->GetBlock(neighborX, neighborY, 0)->IsActive();
    }

    return isDefault;
}

Block *Chunk::GetBlock(int x, int y, int z)
{
    return &m_Blocks[x][y][z];
}