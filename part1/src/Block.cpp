#include "Block.hpp"
#include <iostream>

Block::Block()
{
    m_active = true;
    m_blockType = BlockType_Grass;
}

Block::Block(int x, int y, int z)
{
    m_position = glm::vec3(x, y, z);
    m_blockType = BlockType_Grass;
    m_active = true;
}

Block::~Block()
{
}

bool Block::IsActive()
{
    return m_active;
}

void Block::SetActive(bool isActive)
{
    m_active = isActive;
}

BlockType Block::getType() {
    return m_blockType;
}
