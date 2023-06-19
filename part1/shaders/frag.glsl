#version 410 core

in vec3 v_vertexColors;

out vec4 color;

// Entry point of program
void main()
{
	// Instead of using vertex colors, we will
	// instead output a texture.
	vec4 VertexColors = vec4(v_vertexColors.r,v_vertexColors.g, v_vertexColors.b, 1.0f);

	color = VertexColors;
}