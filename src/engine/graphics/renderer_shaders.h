//
// Created by Ion Agorria on 17/02/19
//
#ifndef OPENE2140_RENDERER_SHADERS_H
#define OPENE2140_RENDERER_SHADERS_H

/**
 * Vertex shader code
 */
const char* VERTEX_SHADER = R"vertex(
#version 330 core

uniform mat4 uCombined;

layout(location = 0) in vec2 attribPosition;
layout(location = 1) in vec4 attrib1vec4;

out vec4 out1vec4;

void main() {
    //Multiply position of vertex with combined matrix, this gives us the final position to render the vertex
    gl_Position = uCombined * vec4(attribPosition.x, attribPosition.y, 0.0, 1.0);
    out1vec4 = attrib1vec4;
}
)vertex";

/**
 * Texture fragment shader code
 */
const char* FRAGMENT_SHADER_TEXTURE = R"fragment(
#version 330 core

uniform sampler2D uTextureImageRGBA;

in vec4 out1vec4;
out vec4 FragColor;

void main() {
    //Get the color from image texture
    FragColor = texture(uTextureImageRGBA, out1vec4.xy);
    //Uncomment to override with texcoord
    //FragColor = vec4(out1vec4.xy, 0.0, 1.0);
}
)fragment";

/**
 * Palette texture fragment shader code
 */
const char* FRAGMENT_SHADER_PALETTE_TEXTURE = R"fragment(
#version 330 core

uniform int uPaletteExtraOffset;
uniform usampler2D uTextureImagePalette;
uniform sampler1D uTexturePalette;
uniform sampler1D uTexturePaletteExtra;

in vec4 out1vec4;
out vec4 FragColor;

void main() {
    //Get the index to access in the palettes from the 2D image
    int index = int(texture(uTextureImagePalette, out1vec4.xy).r);
    if (0 <= uPaletteExtraOffset && uPaletteExtraOffset <= index) {
        //Access the real color from extra palette
        FragColor = texelFetch(uTexturePaletteExtra, index - uPaletteExtraOffset, 0);
    } else {
        //Access the real color from main palette
        FragColor = texelFetch(uTexturePalette, index, 0);
    }
    //Uncomment to override with texcoord
    //FragColor = vec4(out1vec4.xy, 0.0, 1.0);
}
)fragment";

/**
 * Color fragment shader code
 */
const char* FRAGMENT_SHADER_COLOR = R"fragment(
#version 330 core

in vec4 out1vec4;
out vec4 FragColor;

void main() {
    //Get the color from vertex directly
    FragColor = out1vec4;
}
)fragment";

#endif //OPENE2140_RENDERER_SHADERS_H
