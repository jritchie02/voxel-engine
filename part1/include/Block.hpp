#ifndef BLOCK_HPP // include guard
#define BLOCK_HPP

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

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
    Block(int x, int y, int z);
    ~Block();
    bool IsActive();
    void SetActive(bool active);
    glm::vec3 m_position;
    
private:
    bool m_active;
    BlockType m_blockType;
    
};

#endif /* BLOCK_HPP*/