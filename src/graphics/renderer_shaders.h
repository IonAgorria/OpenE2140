//
// Created by Ion Agorria on 17/02/19
//
#ifndef OPENE2140_RENDERER_SHADERS_H
#define OPENE2140_RENDERER_SHADERS_H

#define RENDERER_SHADER_MODE_TEXTURE 0
#define RENDERER_SHADER_MODE_PALETTE_TEXTURE 1

/**
 * Vertex shader code
 */
const char* VERTEX_SHADER_CODE = R"vertex(
#version 330 core

uniform mat4 uniformMatrix;

layout(location = 0) in vec2 attribPosition;
layout(location = 1) in vec2 attribSize;
layout(location = 2) in float attribRotation;
layout(location = 3) in vec4 attribTextureUV;

out VERTEXSHADER_OUT {
    vec2 size;
    float rotation;
    vec4 textureUV;
} vs_out;

void main() {
    //Multiply position of vertex with combined matrix, this gives us the final position to render the vertex
    gl_Position = vec4(attribPosition.x, attribPosition.y, 0.0, 1.0);
    //uniformMatrix *;
    vs_out.size = attribSize;
    vs_out.rotation = attribRotation;
    vs_out.textureUV = attribTextureUV;
}
)vertex";

/**
 * Geometry shader code
 *
 * UV vec4:
 * (x, y,  z,  w)
 * (u, v, u2, v2)
 */
const char* GEOMETRY_SHADER_CODE = R"geometry(
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VERTEXSHADER_OUT {
    vec2 size;
    float rotation;
    vec4 textureUV;
} gs_in[];

out vec2 gs_TexCoord;

void main() {
    float sx = gs_in[0].size.x;
    float sy = gs_in[0].size.y;
    if (gs_in[0].rotation == 0) {
        //Bottom left
        gl_Position = gl_in[0].gl_Position + vec4(-sx, -sy, 0.0, 0.0);
        gs_TexCoord = gs_in[0].textureUV.xy;
        EmitVertex();

        //Bottom right
        gl_Position = gl_in[0].gl_Position + vec4( sx, -sy, 0.0, 0.0);
        gs_TexCoord = gs_in[0].textureUV.zy;
        EmitVertex();

        //Top left
        gl_Position = gl_in[0].gl_Position + vec4(-sx,  sy, 0.0, 0.0);
        gs_TexCoord = gs_in[0].textureUV.xw;
        EmitVertex();

        //Top right
        gl_Position = gl_in[0].gl_Position + vec4( sx,  sy, 0.0, 0.0);
        gs_TexCoord = gs_in[0].textureUV.zw;
        EmitVertex();
    } else {
        float rs = sin(gs_in[0].rotation);
        float rc = cos(gs_in[0].rotation);
        vec4 position = gl_in[0].gl_Position;

        //Bottom left
        gl_Position = gl_in[0].gl_Position + vec4(
            position.x + ((rc * -sx) - (rs * -sy)),
            position.y + ((rs * -sx) + (rc * -sy)),
            0.0, 0.0
        );
        gs_TexCoord = gs_in[0].textureUV.xy;
        EmitVertex();

        //Bottom right
        gl_Position = gl_in[0].gl_Position + vec4(
            position.x + ((rc *  sx) - (rs * -sy)),
            position.y + ((rs *  sx) + (rc * -sy)),
            0.0, 0.0
        );
        gs_TexCoord = gs_in[0].textureUV.zy;
        EmitVertex();

        //Top left
        gl_Position = gl_in[0].gl_Position + vec4(
            position.x + ((rc * -sx) - (rs *  sy)),
            position.y + ((rs * -sx) + (rc *  sy)),
            0.0, 0.0
        );
        gs_TexCoord = gs_in[0].textureUV.xw;
        EmitVertex();

        //Top right
        gl_Position = gl_in[0].gl_Position + vec4(
            position.x + ((rc *  sx) - (rs * sy)),
            position.y + ((rs *  sx) + (rc * sy)),
            0.0, 0.0
        );
        gs_TexCoord = gs_in[0].textureUV.zw;
        EmitVertex();
    }

    EndPrimitive();
}
)geometry";

/**
 * Fragment shader code
 */
const char* FRAGMENT_SHADER_CODE = R"fragment(
#version 330 core

uniform int uMode;
uniform int uPaletteExtraOffset;
uniform usampler2D uTextureImagePalette;
uniform sampler2D uTextureImageRGBA;
uniform sampler1D uTexturePalette;
uniform sampler1D uTexturePaletteExtra;

in vec2 gs_TexCoord;
out vec4 FragColor;

void main() {
    vec4 vColor;
    if (uMode == 1) {
        //Get the index to access in the palettes from the 2D image
        int index = int(texture(uTextureImagePalette, gs_TexCoord).r);
        if (0 <= uPaletteExtraOffset && uPaletteExtraOffset <= index) {
            //Access the real color from extra palette
            vColor = texelFetch(uTexturePaletteExtra, index - uPaletteExtraOffset, 0);
        } else {
            //Access the real color from main palette
            vColor = texelFetch(uTexturePalette, index, 0);
        }
    } else {
        //Get the color from image texture
        vColor = texture2D(uTextureImageRGBA, gs_TexCoord);
    }
    //Discard if alpha is lower than threshold
    //if (vColor.a < uAlphaThreshold) discard;
    FragColor = vColor;
    //Uncomment to override with texcoord
    //FragColor = vec4(gs_TexCoord.xy, 0.0, 1.0);
    //Uncomment to show depth buffer
    //FragColor.xyz = vec3(gl_FragCoord.z / gl_FragCoord.w * 0.4); gl_FragColor.a = 1.0;
}
)fragment";

#endif //OPENE2140_RENDERER_SHADERS_H
