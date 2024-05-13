#version 330 core

in vec4 rgbaColor;
in vec2 texCoords;
in vec2 texDimens;
in vec2 archSize;
in float texStart;

out vec4 color;

uniform sampler2D batchTextures[32];

void main()
{
    float sy = (texStart / archSize.y) + ((texDimens.y / archSize.y) * texCoords.y);
    float sx = (texDimens.x / archSize.x) * texCoords.x;

    float halfX = 0.5 / archSize.x;
    float halfY = 0.5 / archSize.y;

    color = rgbaColor * texelFetch(batchTextures[0], ivec2(halfX + sx * archSize.x, halfY + sy * archSize.y), 0);

    // color = rgbaColor * texture(batchTextures[0], vec2(sx, sy));
}
