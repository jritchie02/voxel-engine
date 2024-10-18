#include "Block.hpp"
#include <iostream>

Block::Block()
{
    m_active = true;
    m_blockType = BlockType_Grass;
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
