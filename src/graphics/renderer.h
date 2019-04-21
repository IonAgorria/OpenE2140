//
// Created by Ion Agorria on 3/11/18
//
#ifndef OPENE2140_RENDERER_H
#define OPENE2140_RENDERER_H

#include <glm/matrix.hpp>
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
     * Max texture size
     */
    int maxTextureSize;

    /**
     * Last used texture for RGBA image
     */
    GLuint lastTextureImageRGBA;

    /**
     * Last used texture for palette image
     */
    GLuint lastTextureImagePalette;

    /**
     * Last used texture for palette
     */
    GLuint lastTexturePalette;

    /**
     * Last used texture for extra palette
     */
    GLuint lastTexturePaletteExtra;

    /**
     * Location for combined uniform in shader
     */
    GLint uCombinedLocation;

    /**
     * Location for uMode uniform in shader
     */
    GLint uModeLocation;

    /**
     * Location for uPaletteExtraOffset uniform in shader
     */
    GLint uPaletteExtraOffsetLocation;

    /**
     * Location for uTextureImagePalette uniform in shader
     */
    GLint uTextureImagePaletteLocation;

    /**
     * Location for uTextureImageRGBA uniform in shader
     */
    GLint uTextureImageRGBALocation;

    /**
     * Location for uTexturePalette uniform in shader
     */
    GLint uTexturePaletteLocation;

    /**
     * Location for uTexturePaletteExtra uniform in shader
     */
    GLint uTexturePaletteExtraLocation;

    /**
     * Current mode value for shaders
     */
    int mode;

    /**
     * Current projection matrix
     */
    glm::mat4 projection;

    /**
     * Current view matrix
     */
    glm::mat4 view;
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
     * Updates viewport for renderer
     *
     * @param width
     * @param height
     */
    void changeViewport(int width, int height);

    /**
     * Updates camera for renderer
     *
     * @param width
     * @param height
     */
    void changeCamera(int x, int y);

    /**
     * Draws the provided data
     *
     * @param x position of drawn image
     * @param y position of drawn image
     * @param width size of drawn image
     * @param height size of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void draw(float x, float y, float width, float height, float angle, Image& image, Palette* paletteExtra);

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

    /**
     * @return the maximum texture size allowed
     */
    unsigned int getMaxTextureSize();
};

#endif //OPENE2140_RENDERER_H
