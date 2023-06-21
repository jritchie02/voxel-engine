#include "ChunkManager.hpp"

ChunkManager::ChunkManager()
{
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};

    // Initialize the m_ChunkGrid vector
    m_ChunkGrid.resize(CHUNK_GRID_SIZE, std::vector<Chunk>(CHUNK_GRID_SIZE));

    // Iterate over x, y coordinates to initialize each chunk
    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_GRID_SIZE; ++y)
        {
            // Generate terrain using Perlin noise for each chunk
            Chunk chunk = Chunk(perlin, x, y);

            // Assign the generated chunk to the chunk grid
            m_ChunkGrid[x][y] = chunk;
        }
    }
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::GenerateChunks()
{
}

const std::vector<GLfloat> ChunkManager::get_vertex_data()
{
    std::vector<GLfloat> vertices;

    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_GRID_SIZE; ++z)
        {
            std::vector<GLfloat> chunkVertices = m_ChunkGrid[x][z].get_vertex_data(x, z);
            vertices.insert(vertices.end(), chunkVertices.begin(), chunkVertices.end());
        }
    }

    return vertices;
}

const std::vector<GLuint> ChunkManager::get_index_data()
{
    std::vector<GLuint> indexes;
    GLuint baseIndex = 0;
    for (int x = 0; x < CHUNK_GRID_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_GRID_SIZE; ++y)
        {
            // pass in the indexCounter to getIndexData to keep count each iteration
            std::vector<GLuint> chunkIndexes = m_ChunkGrid[x][y].get_index_data(baseIndex);
            indexes.insert(indexes.end(), chunkIndexes.begin(), chunkIndexes.end());
        }
    }

    return indexes;
}

// void ChunkManager::RenderChunks()
//{
//
// }

void ChunkManager::UpdateChunks(float dt)
{
}