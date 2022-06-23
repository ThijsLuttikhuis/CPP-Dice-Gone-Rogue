#version 420 core
in vec2 TexCoords;
out vec4 color;

layout (depth_greater) out float gl_FragDepth;
uniform float zIndex;

uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform float spriteAlpha;

void main()
{
    color = vec4(spriteColor, spriteAlpha) * texture(sprite, TexCoords);
    //color = color + 32.0/2/256 - mod(256 * color, 32) / 256;
    if (color.a <= 0.0) {
        discard;
    }
    gl_FragDepth = zIndex;
}
