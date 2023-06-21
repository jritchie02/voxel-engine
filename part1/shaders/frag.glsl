#version 410 core

// Uniform variables
uniform vec3 u_LightDirection;

// Pass vertex colors and normal from the vertex shader
in vec3 v_vertexColors;
in vec3 v_normal;

out vec4 color;

void main()
{
    // Calculate the dot product between the light direction and the surface normal
    float diffuse = max(dot(normalize(v_normal), normalize(u_LightDirection)), 0.05);

    // Apply the diffuse color and intensity to the fragment color
    color = vec4(v_vertexColors * diffuse, 1.0);
}