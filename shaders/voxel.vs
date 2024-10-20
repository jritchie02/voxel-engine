#version 410 core
layout(location = 0) in vec3 aPos;
layout(location=1) in vec2 textureCoords;
layout(location=2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Pass vertex colors to the fragment shader
out vec2 v_textureCoords;
out vec3 v_normal;
out vec3 v_fragPos;

void main()
{
    v_textureCoords = textureCoords;
    v_fragPos = vec3(model * vec4(aPos, 1.0));
    v_normal = aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}