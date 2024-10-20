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
    m_xOffset = xOffset * CHUNK_SIZE;
    m_zOffset = zOffset * CHUNK_SIZE;
    // Initialize the m_Blocks vector
    m_Voxels.resize(CHUNK_SIZE, std::vector<std::vector<Voxel>>(CHUNK_SIZE, std::vector<Voxel>(CHUNK_SIZE)));
    // Iterate over x, y, z coordinates to initialize each Voxel
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

                // Create and assign a new Voxel
                Voxel v(glm::vec3(x + m_xOffset, y, z + m_zOffset), 1, glm::vec2(9, 7));
                m_Voxels[x][y][z] = v;

                if (noise < threshold)
                {
                    m_Voxels[x][y][z].SetActive(false);
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

    std::cout << "Getting Voxel at " << x << ", " << y << ", " << z << std::endl;

    if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE ||
        x < 0 || y < 0 || z < 0)
    {
        std::cout << "Voxel is out of bounds " << std::endl;
    }
    else
    {
        m_Voxels[x][y][z].SetActive(isActive);
    }
}

const std::vector<GLfloat> Chunk::GetVertexData(int xOffset, int zOffset, std::vector<GLuint> &indices, GLuint &baseIndex)
{
    std::vector<GLfloat> vertices;

    xOffset = xOffset * CHUNK_SIZE;
    zOffset = zOffset * CHUNK_SIZE;

    // Iterate over the blocks in the chunk
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                Voxel currBlock = m_Voxels[x][y][z];
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
    bool result = (x >= m_xOffset && x < m_xOffset + (CHUNK_SIZE) &&
                   z >= m_zOffset && z < m_zOffset + (CHUNK_SIZE));
    int xOffsetTotal = m_xOffset + (CHUNK_SIZE);
    int zOffsetTotal = m_zOffset + (CHUNK_SIZE);

    return result;
}

std::vector<GLfloat> Chunk::GenerateCubeVertices(int x, int y, int z, std::vector<GLuint> &indices, GLuint &baseIndex)
{
    Voxel voxel = m_Voxels[x][y][z];

    // Calculate the coordinates of the cube's vertices based on the Voxel position
    std::vector<GLfloat> vertices;

    // Front face
    if (!HasNeighborOnFace(x, y, z, 0, 0, 1))
    {
        std::vector<GLfloat> frontVertices = voxel.GetFrontVertices();
        vertices.insert(vertices.end(), frontVertices.begin(), frontVertices.end());

        // Add the face indices (update this depending on how your indices are structured)
        AddFace(indices, baseIndex);
    }

    // Back face
    if (!HasNeighborOnFace(x, y, z, 0, 0, -1))
    {
        std::vector<GLfloat> frontVertices = voxel.GetBackVertices();
        vertices.insert(vertices.end(), frontVertices.begin(), frontVertices.end());

        // Add the face indices (update this depending on how your indices are structured)
        AddFace(indices, baseIndex);
    }

    // Left face
    if (!HasNeighborOnFace(x, y, z, -1, 0, 0))
    {
        std::vector<GLfloat> frontVertices = voxel.GetLeftVertices();
        vertices.insert(vertices.end(), frontVertices.begin(), frontVertices.end());

        // Add the face indices (update this depending on how your indices are structured)
        AddFace(indices, baseIndex);
    }

    // Right face
    if (!HasNeighborOnFace(x, y, z, 1, 0, 0))
    {
        std::vector<GLfloat> frontVertices = voxel.GetRightVertices();
        vertices.insert(vertices.end(), frontVertices.begin(), frontVertices.end());

        // Add the face indices (update this depending on how your indices are structured)
        AddFace(indices, baseIndex);
    }

    // Top face
    if (!HasNeighborOnFace(x, y, z, 0, 1, 0))
    {
        std::vector<GLfloat> frontVertices = voxel.GetTopVertices();
        vertices.insert(vertices.end(), frontVertices.begin(), frontVertices.end());

        // Add the face indices (update this depending on how your indices are structured)
        AddFace(indices, baseIndex);
    }

    // Bottom face
    if (!HasNeighborOnFace(x, y, z, 0, -1, 0))
    {
        std::vector<GLfloat> frontVertices = voxel.GetBottomVertices();
        vertices.insert(vertices.end(), frontVertices.begin(), frontVertices.end());

        // Add the face indices (update this depending on how your indices are structured)
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
        return m_Voxels[neighborX][neighborY][neighborZ].IsActive();
    }

    // Check neighboring chunks if the position is on an edge case
    if (neighborX < 0 && m_rightNeighbor != nullptr)
    {
        return m_rightNeighbor->GetVoxel(CHUNK_SIZE - 1, neighborY, neighborZ)->IsActive();
    }

    else if (neighborX >= CHUNK_SIZE && m_leftNeighbor != nullptr)
    {
        return m_leftNeighbor->GetVoxel(0, neighborY, neighborZ)->IsActive();
    }

    if (neighborZ < 0 && m_backNeighbor != nullptr)
    {
        return m_backNeighbor->GetVoxel(neighborX, neighborY, CHUNK_SIZE - 1)->IsActive();
    }
    else if (neighborZ >= CHUNK_SIZE && m_frontNeighbor != nullptr)
    {
        return m_frontNeighbor->GetVoxel(neighborX, neighborY, 0)->IsActive();
    }

    return isDefault;
}

Voxel *Chunk::GetVoxel(int x, int y, int z)
{
    return &m_Voxels[x][y][z];
}