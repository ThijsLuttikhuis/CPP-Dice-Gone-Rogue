#version 420 core
in vec2 TexCoords;
out vec4 color;

layout (depth_greater) out float gl_FragDepth;
uniform float zIndex;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
    if (color.a <= 0.0) {
        discard;
    }
    gl_FragDepth = zIndex;
}
