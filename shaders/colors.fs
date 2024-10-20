#version 410 core
out vec4 FragColor;

uniform vec3 lightColor;  // Light color
uniform sampler2D u_Texture;  // Texture sampler
uniform vec3 lightPos;  
uniform vec3 viewPos;

in vec2 v_textureCoords;  // Texture coordinates
in vec3 v_normal;  // Normal vector
in vec3 v_fragPos;  // Fragment position

void main()
{
    // Ambient lighting calculation
    float ambientStrength = 0.3;  // Control ambient intensity
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting calculation
    vec3 norm = normalize(v_normal);
    // Calculate the light direction relative to the fragment position
    vec3 lightDir = normalize(lightPos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting calculation
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - v_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;


    // Sample the texture color
    vec4 texColor = texture(u_Texture, v_textureCoords);

    // Combine ambient lighting with the texture color
    vec3 resultColor = texColor.rgb * (ambient + diffuse + specular);

    // Set the final fragment color, preserving the texture's alpha
    FragColor = vec4(resultColor, texColor.a);
}