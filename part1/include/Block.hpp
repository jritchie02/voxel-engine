#ifndef BLOCK_HPP // include guard
#define BLOCK_HPP

#include <vector>
#include <glad/glad.h>

class Block
{
    enum BlockType
    {
        BlockType_Default = 0,
        BlockType_Grass,
        BlockType_Dirt,
        BlockType_Water,
        BlockType_Stone,
        BlockType_Wood,
        BlockType_Sand,
        BlockType_NumTypes,
    };

public:
    Block();
    ~Block();
    bool IsActive();
    void SetActive(bool active);

    const std::vector<GLfloat> get_vertex_data();
    const std::vector<GLuint> get_index_data();

private:
    std::vector<GLfloat> m_vertices;
     std::vector<GLuint> m_indices;
    bool m_active;
    BlockType m_blockType;

};

#endif /* BLOCK_HPP*/