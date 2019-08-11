//
// Created by Ion Agorria on 17/02/19
//
#ifndef OPENE2140_RENDERER_SHADERS_H
#define OPENE2140_RENDERER_SHADERS_H

/*
 * UV vec4:
 * (x, y,  z,  w)
 * (u, v, u2, v2)
 */

/**
 * Vertex shader code
 */
const char* VERTEX_SHADER = R"vertex(
#version 330 core

layout(location = 0) in vec2 attribPosition;
layout(location = 1) in vec4 attribTextureUV;
layout(location = 2) in vec2 attrib2vec2;
layout(location = 3) in float attrib3float;

out VERTEXSHADER_OUT {
    vec4 textureUV;
    vec2 attrib2vec2;
    float attrib3float;
} vs_out;

void main() {
    //Multiply position of vertex with combined matrix, this gives us the final position to render the vertex
    gl_Position = vec4(attribPosition.x, attribPosition.y, 0.0, 1.0);
    vs_out.textureUV = attribTextureUV;
    vs_out.attrib2vec2 = attrib2vec2;
    vs_out.attrib3float = attrib3float;
}
)vertex";

/**
 * Rectangle geometry shader code
 */
const char* GEOMETRY_SHADER_RECTANGLE = R"geometry(
#version 330 core

uniform mat4 uCombined;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VERTEXSHADER_OUT {
    vec4 textureUV;
    vec2 attrib2vec2;
    float attrib3float;
} gs_in[];

out vec2 gs_TexCoord;

void main() {
    float sx = gs_in[0].attrib2vec2.x;
    float sy = gs_in[0].attrib2vec2.y;
    vec4 position = gl_in[0].gl_Position;
    if (gs_in[0].attrib3float == 0) {
        //Non rotated rectangle mode

        //Bottom left
        gl_Position = uCombined * (position + vec4(-sx, -sy, 0.0, 0.0));
        gs_TexCoord = gs_in[0].textureUV.xy;
        EmitVertex();

        //Bottom right
        gl_Position = uCombined * (position + vec4( sx, -sy, 0.0, 0.0));
        gs_TexCoord = gs_in[0].textureUV.zy;
        EmitVertex();

        //Top left
        gl_Position = uCombined * (position + vec4(-sx,  sy, 0.0, 0.0));
        gs_TexCoord = gs_in[0].textureUV.xw;
        EmitVertex();

        //Top right
        gl_Position = uCombined * (position + vec4( sx,  sy, 0.0, 0.0));
        gs_TexCoord = gs_in[0].textureUV.zw;
        EmitVertex();
    } else {
        //Rotated rectangle mode

        float rs = sin(gs_in[0].attrib3float);
        float rc = cos(gs_in[0].attrib3float);

        //Bottom left
        gl_Position = uCombined * (position + vec4(
            (rc * -sx) - (rs * -sy),
            (rs * -sx) + (rc * -sy),
            0.0, 0.0
        ));
        gs_TexCoord = gs_in[0].textureUV.xy;
        EmitVertex();

        //Bottom right
        gl_Position = uCombined * (position + vec4(
            (rc *  sx) - (rs * -sy),
            (rs *  sx) + (rc * -sy),
            0.0, 0.0
        ));
        gs_TexCoord = gs_in[0].textureUV.zy;
        EmitVertex();

        //Top left
        gl_Position = uCombined * (position + vec4(
            (rc * -sx) - (rs *  sy),
            (rs * -sx) + (rc *  sy),
            0.0, 0.0
        ));
        gs_TexCoord = gs_in[0].textureUV.xw;
        EmitVertex();

        //Top right
        gl_Position = uCombined * (position + vec4(
            (rc *  sx) - (rs * sy),
            (rs *  sx) + (rc * sy),
            0.0, 0.0
        ));
        gs_TexCoord = gs_in[0].textureUV.zw;
        EmitVertex();
    }

    EndPrimitive();
}
)geometry";

/**
 * Line geometry shader code
 */
const char* GEOMETRY_SHADER_LINE = R"geometry(
#version 330 core

uniform mat4 uCombined;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VERTEXSHADER_OUT {
    vec4 textureUV;
    vec2 attrib2vec2;
    float attrib3float;
} gs_in[];

out vec2 gs_TexCoord;

void main() {
    float sx = gs_in[0].gl_Position.x;
    float sy = gs_in[0].gl_Position.y;
    float ex = gs_in[0].attrib2vec2.x;
    float ey = gs_in[0].attrib2vec2.y;

    EndPrimitive();
}
)geometry";

/**
 * Texture fragment shader code
 */
const char* FRAGMENT_SHADER_TEXTURE = R"fragment(
#version 330 core

uniform sampler2D uTextureImageRGBA;

in vec2 gs_TexCoord;
out vec4 FragColor;

void main() {
    //Get the color from image texture
    FragColor = texture(uTextureImageRGBA, gs_TexCoord);
    //Uncomment to override with texcoord
    //FragColor = vec4(gs_TexCoord.xy, 0.0, 1.0);
}
)fragment";

/**
 * Palette texture fragment shader code
 */
const char* FRAGMENT_SHADER_PALETTE_TEXTURE = R"fragment(
#version 330 core

uniform int uMode;
uniform int uPaletteExtraOffset;
uniform usampler2D uTextureImagePalette;
uniform sampler1D uTexturePalette;
uniform sampler1D uTexturePaletteExtra;

in vec2 gs_TexCoord;
out vec4 FragColor;

void main() {
    //Get the index to access in the palettes from the 2D image
    int index = int(texture(uTextureImagePalette, gs_TexCoord).r);
    if (0 <= uPaletteExtraOffset && uPaletteExtraOffset <= index) {
        //Access the real color from extra palette
        FragColor = texelFetch(uTexturePaletteExtra, index - uPaletteExtraOffset, 0);
    } else {
        //Access the real color from main palette
        FragColor = texelFetch(uTexturePalette, index, 0);
    }
    //Uncomment to override with texcoord
    //FragColor = vec4(gs_TexCoord.xy, 0.0, 1.0);
}
)fragment";

#endif //OPENE2140_RENDERER_SHADERS_H
