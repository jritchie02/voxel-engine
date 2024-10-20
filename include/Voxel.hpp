#ifndef VOXEL_HPP
#define VOXEL_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

class Voxel
{
public:
    Voxel() = default;
    // Position should be the corner of the voxel
    Voxel(glm::vec3 position, float width, glm::vec2 texture_position);

    // Methods to get list of verticies and indexes
    std::vector<GLfloat> GetVertexData();
    std::vector<GLfloat> GetFrontVertices();
    std::vector<GLfloat> GetBackVertices();
    std::vector<GLfloat> GetLeftVertices();
    std::vector<GLfloat> GetRightVertices();
    std::vector<GLfloat> GetTopVertices();
    std::vector<GLfloat> GetBottomVertices();

    void GetIndexData(std::vector<GLuint> &indices, GLuint &nextIndex);

    glm::vec3 GetPosition();

    void SetPosition(glm::vec3 position) { m_position = position; }

    void SetActive(bool active) { m_active = active; }

    bool IsActive() { return m_active; }

private:
    glm::vec3 m_position;
    glm::vec2 m_texture_position;
    bool m_active = true;

    float m_width = 1.0f;

    glm::vec4 GetTextureCoordinates(int x, int y);
};

#endif /* VOXEL_HPP */