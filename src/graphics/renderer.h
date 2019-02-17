//
// Created by Ion Agorria on 3/11/18
//
#ifndef OPENE2140_RENDERER_H
#define OPENE2140_RENDERER_H

#include "core/macros.h"
#include "core/errorpossible.h"
#include "io/log.h"
#include "image.h"

#define MAX_BATCH_VERTICES 10240
#define MAX_COMPONENTS_PER_VERTICE 9

/**
 * Handles the rendering of various parts using window and game state
 */
class Renderer: public IErrorPossible {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Shader program handle
     */
    GLuint programHandle;

    /**
     * Vertex shader handle
     */
    GLuint programVertexHandle;

    /**
     * Geometry shader handle
     */
    GLuint programGeometryHandle;

    /**
     * Fragment shader handle
     */
    GLuint programFragmentHandle;

    /**
     * VAO buffer handle
     */
    GLuint vaoHandle;

    /**
     * VBO buffer handle
     */
    GLuint vboHandle;

    /**
     * Vertices buffer to store current vertices in batch
     */
    GLfloat vertices[MAX_BATCH_VERTICES * MAX_COMPONENTS_PER_VERTICE];

    /**
     * Vertices buffer written vertices count
     */
    unsigned int verticesCount;

    /**
     * Vertices buffer index
     */
    unsigned int verticesIndex;

    /**
     * Last used texture
     */
    GLuint lastTexture;
public:
    /**
     * Constructs loader
     */
    Renderer();

    /**
     * Destructs loader and cleans any loaded assets
     */
    ~Renderer();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Renderer)

    /**
     * Flushes data if any to GPU
     */
    bool flush();

    /**
     * Draws the provided data
     */
    void draw(float x, float y, float width, float height, float angle, Image& image);

    /**
     * Create and load a OpenGL shader.
     *
     * @param type Shader type to create.
     * @param code The shader code.
     * @return id for the shader
     */
    GLuint loadShader(GLenum type, const char* code);

    /**
     * Initializes OpenGL shader program with shaders code.
     */
    void initShaderProgram();

    /**
     * Initializes OpenGL buffers.
     */
    void initBuffers();
};

#endif //OPENE2140_RENDERER_H
