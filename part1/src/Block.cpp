#include "Block.hpp"
#include <iostream>

Block::Block() {

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
