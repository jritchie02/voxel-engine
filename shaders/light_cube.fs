#version 410 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 v_vertexColors;
in vec2 v_textureCoords;

uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, v_textureCoords);
}