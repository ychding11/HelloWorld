#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out VS_OUT
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 posInWorld;
    vec4 posInWorldLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoords = texCoords;
    vs_out.posInWorld = vec3(model * vec4(position, 1.0));
    vs_out.posInWorldLightSpace = lightSpaceMatrix * vec4(vs_out.posInWorld, 1.0);
}