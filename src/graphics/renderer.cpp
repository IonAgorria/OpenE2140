//
// Created by Ion Agorria on 3/11/18
//
#include "src/core/utils.h"
#include "renderer.h"

/**
 * Vertex shader code
 */
const char* VERTEX_SHADER_CODE = R"vertex(
#version 150 core

uniform mat4 uMatrix;
attribute vec3 aPosition;
attribute vec2 aTexCoord;
varying vec2 varTexCoord;

void main() {
    //Pass the texture coordinates to fragment shader via varying
    varTexCoord = aTexCoord;
    //Multiply position of vertex with combined matrix, this gives us the final position to render the vertex
    gl_Position = uMatrix * vec4(aPosition.xyz, 1.0);
}
)vertex";

/**
 * Geometry shader code
 */
const char* GEOMETRY_SHADER_CODE = R"geometry(
#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}
)geometry";

/**
 * Fragment shader code
 */
const char* FRAGMENT_SHADER_CODE = R"fragment(
#version 150 core

precision mediump float;
uniform sampler2D uTexture;
uniform vec4 uColor;
uniform float uAlphaThreshold;
varying vec2 varTexCoord;

void main() {
    //Apply the texture id and coordinates, then multiply it with color
    vec4 vColor = texture2D(uTexture, varTexCoord) * uColor;
    //Uncomment to override with color
    //vColor = uColor;
    //Discard if alpha is lower than threshold
    if (vColor.a < uAlphaThreshold) discard;
    //Apply the texture id and coordinates, then multiply it with color
    gl_FragColor = vColor;
    //Uncomment to override with texcoord
    //gl_FragColor = vec4(varTexCoord, 0.0, 1.0);
    //Uncomment to show depth buffer
    //gl_FragColor.xyz = vec3(gl_FragCoord.z / gl_FragCoord.w * 0.4); gl_FragColor.a = 1.0;
}
)fragment";

Renderer::Renderer() {
    log = Log::get("Renderer");
    initShaderProgram();
    if (!error.empty()) return;
}

Renderer::~Renderer() {
    log->debug("Closing");
    if (programHandle) {
        glUseProgram(0);
        glDetachShader(programHandle, programVertexHandle);
        glDetachShader(programHandle, programGeometryHandle);
        glDetachShader(programHandle, programFragmentHandle);
        glDeleteProgram(programHandle);
        programHandle = 0;
    }
    if (programVertexHandle) {
        glDeleteShader(programVertexHandle);
        programVertexHandle = 0;
    }
    if (programGeometryHandle) {
        glDeleteShader(programGeometryHandle);
        programGeometryHandle = 0;
    }
    if (programFragmentHandle) {
        glDeleteShader(programFragmentHandle);
        programFragmentHandle = 0;
    }
}

GLuint Renderer::loadShader(GLenum type, const char* code) {
    log->debug("Loading shader type {0}", type);

    //Create shader
    GLuint shader = glCreateShader(type);
    error = Utils::checkGLError(log);
    if (!error.empty()) return 0;

    //Add the source code to the shader and compile it
    glShaderSource(shader, 1, &code, NULL);
    error = Utils::checkGLError(log);
    if (!error.empty()) return 0;
    glCompileShader(shader);
    error = Utils::checkGLError(log);
    if (!error.empty()) return 0;

    //Get info log
    int logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char buffer[logSize];
    glGetShaderInfoLog(shader, logSize, &logSize, buffer);
    std::string shaderLog(buffer);
    if (!shaderLog.empty()) {
        if (Utils::toLower(shaderLog).find("error")) {
            log->error("Shader type {0} compile error: {1}", type, shaderLog);
            return 0;
        } else {
            log->error("Shader type {0} compile log: {1}", type, shaderLog);
        }
    }

    return shader;
}

void Renderer::initShaderProgram() {
    log->debug("Initializing shader program");
    //Load shaders
    programVertexHandle = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_CODE);
    if (!error.empty()) return;
    programGeometryHandle = loadShader(GL_GEOMETRY_SHADER, GEOMETRY_SHADER_CODE);
    if (!error.empty()) return;
    programFragmentHandle = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_CODE);
    if (!error.empty()) return;

    //Create program
    programHandle = glCreateProgram();
    error = Utils::checkGLError(log);
    if (!error.empty()) {
        programHandle = 0;
        return;
    }

    //Attach shaders
    glAttachShader(programHandle, programVertexHandle);
    glAttachShader(programHandle, programGeometryHandle);
    glAttachShader(programHandle, programFragmentHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Bind attributes
    glBindAttribLocation(programHandle, 0, "aPosition");
    glBindAttribLocation(programHandle, 1, "aTexCoord");
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Link the program
    glLinkProgram(programHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Get handle to shader variables
    /*
    positionHandle = GLES20.glGetAttribLocation(programHandle, "aPosition");
    texCoordHandle = GLES20.glGetAttribLocation(programHandle, "aTexCoord");
    error = Utils::checkGLError(log);
    if (!error.empty()) return;
    matrixHandle = GLES20.glGetUniformLocation(programHandle, "uMatrix");
    textureHandle = GLES20.glGetUniformLocation(programHandle, "uTexture");
    colorHandle = GLES20.glGetUniformLocation(programHandle, "uColor");
    alphaThresholdHandle = GLES20.glGetUniformLocation(programHandle, "uAlphaThreshold");
    error = Utils::checkGLError(log);
    if (!error.empty()) return;
    */
}

void Renderer::begin() {

}

void Renderer::end() {

}
