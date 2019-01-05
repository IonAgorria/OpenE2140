//
// Created by Ion Agorria on 3/11/18
//
#ifndef OPENE2140_RENDERER_H
#define OPENE2140_RENDERER_H

#include "core/macros.h"
#include "core/errorpossible.h"
#include "io/log.h"

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
     * Starts the rendering batch
     */
    void begin();

    /**
     * Finishes the current rendering batch
     */
    void end();

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
};

#endif //OPENE2140_RENDERER_H
