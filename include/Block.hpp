#ifndef BLOCK_HPP // include guard
#define BLOCK_HPP

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

enum BlockType
{
    BlockType_Default = 0,
    BlockType_Grass,
    BlockType_Dirt,
};

class Block
{
public:
    // Constructors/Destructors
    Block();
    Block(int x, int y, int z);
    ~Block();

    // Methods
    bool IsActive();
    void SetActive(bool active);
    BlockType getType();

private:
    bool m_active;
    BlockType m_blockType;
};

#endif /* BLOCK_HPP*/