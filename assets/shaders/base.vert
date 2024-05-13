#version 330

layout (location = 0) in vec3 vertPosCoords;
layout (location = 1) in vec4 vertRgbaColor;
layout (location = 2) in vec2 vertTexCoords;
layout (location = 3) in vec2 vertTexDimens;
layout (location = 4) in float vertTexStart;

out vec4 rgbaColor;
out vec2 texCoords;
out vec2 texDimens;
out vec2 archSize;
out float texStart;

uniform vec2 vertArchSize;
uniform mat4 MVP;

void main()
{
    rgbaColor = vertRgbaColor;
    texCoords = vertTexCoords;
    texDimens = vertTexDimens;
    archSize = vertArchSize;
    texStart = vertTexStart;
    
    gl_Position = MVP * vec4(vertPosCoords, 1.0);
}
