#version 410 core
// From Vertex Buffer Object (VBO)
layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

// Uniform variables
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
// We'll use a perspective projection
uniform mat4 u_Projection; 
uniform vec3 u_LightDirection;

// Pass vertex colors to the fragment shader
out vec3 v_vertexColors;
out vec3 v_normal;

void main()
{
    v_vertexColors = vertexColors;

    v_normal = position;

    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
}
