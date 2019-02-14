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

uniform mat4 uniformMatrix;

in vec2 attribPosition;
in vec2 attribSize;
in float attribRotation;
in vec4 attribTextureUV;

out vec4 vertexPosition;
out vec2 vertexSize;
out float vertexRotation;
out vec4 vertexTextureUV;

void main() {
    //Multiply position of vertex with combined matrix, this gives us the final position to render the vertex
    vertexPosition = uniformMatrix * vec4(attribPosition.xy, 1.0, 1.0);
    vertexSize = attribSize;
    vertexRotation = attribRotation;
    vertexTextureUV = attribTextureUV;
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
    initBuffers();
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
    if (vboHandle) {
        glDeleteBuffers(1, &vboHandle);
        vboHandle = 0;
    }
    if (vaoHandle) {
        glDeleteVertexArrays(1, &vaoHandle);
        vaoHandle = 0;
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
            error = "Shader type " + std::to_string(type) + " compile error: " + shaderLog;
            return 0;
        } else {
            log->debug("Shader type {0} compile log: {1}", type, shaderLog);
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

    //Link the program
    glLinkProgram(programHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Delete shaders after linking
    glDeleteShader(programVertexHandle);
    glDeleteShader(programGeometryHandle);
    glDeleteShader(programFragmentHandle);
    programVertexHandle = 0;
    programGeometryHandle = 0;
    programFragmentHandle = 0;
    error = Utils::checkGLError(log);
    if (!error.empty()) return;
}

void Renderer::initBuffers() {
    //Generate buffers
    glGenVertexArrays(1, &vaoHandle);
    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(vaoHandle);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
}

void Renderer::begin() {
    verticesIndex = 0;
}

void Renderer::end() {

}
