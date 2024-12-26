#version 330

layout(location = 0) in vec3 spatialCoords;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoords;

out vec4 inColor;
out vec2 inTexCoords;

uniform mat4 mvp;

void main()
{
	inColor = color;
	inTexCoords = texCoords;
	gl_Position = mvp * vec4(spatialCoords, 1.0);
}