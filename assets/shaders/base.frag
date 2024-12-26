#version 330 core

in vec4 inColor;
in vec2 inTexCoords;

out vec4 color;

uniform sampler2D textures[32];

void main()
{
	color = inColor * texture(textures[0], inTexCoords).rgba;
}