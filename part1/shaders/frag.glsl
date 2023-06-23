#version 410 core

// Pass vertex colors and normal from the vertex shader
in vec3 v_vertexColors;
in vec3 v_normal;

out vec4 color;

void main()
{
    vec3 normal = v_normal;

    vec3 lightDirection = vec3(0.0f, .05f, 0.0f);
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseColor = v_vertexColors;
    vec3 finalColor = diffuseIntensity * diffuseColor;
    color = vec4(finalColor, 1.0);
}