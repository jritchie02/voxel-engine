#include "ChunkManager.hpp"
#include <iostream>

ChunkManager::ChunkManager()
{
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    // Initialize the m_ChunkGrid vector
    m_ChunkGrid.resize(CHUNK_GRID_SIZE, std::vector<Chunk *>(CHUNK_GRID_SIZE));

    // Iterate over x, y coordinates to initialize each chunk
    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_GRID_SIZE; ++y)
        {
            // Generate terrain using Perlin noise for each chunk
            Chunk *chunk = new Chunk(perlin, x, y);

            // Assign the generated chunk to the chunk grid
            m_ChunkGrid[x][y] = chunk;
        }
    }

    // Iterate over x, y coordinates to initialize each chunk
    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_GRID_SIZE; ++y)
        {
            // Generate terrain using Perlin noise for each chunk
            Chunk *chunk = m_ChunkGrid[x][y];

            // Set the pointers to the neighboring chunks
            if (y > 0)
            {
                chunk->SetBackNeighbor(m_ChunkGrid[x][y - 1]);
            }

            if (y < CHUNK_GRID_SIZE - 1)
            {
                chunk->SetFrontNeighbor(m_ChunkGrid[x][y + 1]);
            }

            if (x > 0)
            {
                chunk->SetRightNeighbor(m_ChunkGrid[x - 1][y]);
            }

            if (x < CHUNK_GRID_SIZE - 1)
            {
                chunk->SetLeftNeighbor(m_ChunkGrid[x + 1][y]);
            }
        }
    }
}

ChunkManager::~ChunkManager()
{
    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_GRID_SIZE; ++z)
        {
            delete m_ChunkGrid[x][z];
        }
    }
}

void ChunkManager::GenerateChunks()
{
}

const std::vector<GLfloat> ChunkManager::GetVertexData()
{
    std::vector<GLfloat> vertices;
    GLuint baseIndex = 0;

    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_GRID_SIZE; ++z)
        {
            std::vector<GLfloat> chunkVertices = m_ChunkGrid[x][z]->GetVertexData(x, z, m_indexes, baseIndex);
            vertices.insert(vertices.end(), chunkVertices.begin(), chunkVertices.end());
        }
    }

    return vertices;
}

const std::vector<GLuint> ChunkManager::GetIndexData()
{

    return m_indexes;
}

void ChunkManager::UpdateChunks(int x, int y, int z)
{
    bool blockUpdated = false; 

    for (int j = 0; j < CHUNK_GRID_SIZE; ++j)
    {
        for (int m = 0; m < CHUNK_GRID_SIZE; ++m)
        {
            if (m_ChunkGrid[j][m]->IsInBounds(x, y, z))
            {
                std::cout << "In Chunk " << j << ", " << m << std::endl;
                m_ChunkGrid[j][m]->UpdateBlock(x, y, z, false);
                blockUpdated = true;
                break;
            }
        }
        
        if (blockUpdated)
        {
            break;  // Break out of the upper for loop
        }
    }
}