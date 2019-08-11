//
// Created by Ion Agorria on 3/11/18
//
#ifndef OPENE2140_RENDERER_H
#define OPENE2140_RENDERER_H

#include <glm/matrix.hpp>
#include "engine/core/macros.h"
#include "engine/core/error_possible.h"
#include "engine/io/log.h"
#include "image.h"

#define PROGRAM_RECTANGLE_TEXTURE 0
#define PROGRAM_RECTANGLE_PALETTE_TEXTURE 1
#define PROGRAM_LINE_TEXTURE 2
#define PROGRAM_COUNT 3
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
     * Shader program handles
     */
    GLuint programHandles[PROGRAM_COUNT];

    /**
     * Vertex shader handle
     */
    GLuint programVertexHandle;

    /**
     * Rectangle geometry shader handle
     */
    GLuint programGeometryRectangleHandle;

    /**
     * Line geometry shader handle
     */
    GLuint programGeometryLineHandle;

    /**
     * Texture fragment shader handle
     */
    GLuint programFragmentTextureHandle;

    /**
     * Palette texture fragment shader handle
     */
    GLuint programFragmentPaletteTextureHandle;

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
     * Locations for combined uniform in shader
     */
    GLint uCombinedLocations[PROGRAM_COUNT];

    /**
     * Location for uPaletteExtraOffset uniform in shader
     */
    GLint uPaletteExtraOffsetLocation;

    /**
     * Location for uTextureImagePalette uniform in shader
     */
    GLint uTextureImagePaletteLocation;

    /**
     * Locations for uTextureImageRGBA uniform in shader
     */
    GLint uTextureImageRGBALocations[PROGRAM_COUNT];

    /**
     * Location for uTexturePalette uniform in shader
     */
    GLint uTexturePaletteLocation;

    /**
     * Location for uTexturePaletteExtra uniform in shader
     */
    GLint uTexturePaletteExtraLocation;

    /**
     * Current active program
     */
    int activeProgram;

    /**
     * Current projection matrix
     */
    glm::mat4 projection;

    /**
     * Current view matrix
     */
    glm::mat4 view;

    /**
     * Viewport rectangle
     */
    Rectangle viewport;

    /**
     * Prepares the internal states to draw the provided objects
     *
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     * @param line specified if preparations are for line mode
     */
    void prepare(const Image& image, const Palette* paletteExtra, bool line);
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
     * @param x
     * @param y
     * @param width
     * @param height
     */
    void changeViewport(int x, int y, int width, int height);

    /**
     * @return current renderer viewport
     */
    const Rectangle& getViewport() const;

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
    void draw(float x, float y, float width, float height, float angle, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided data
     *
     * @param position of drawn image
     * @param size of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void draw(const Vector2& position, const Vector2& size, float angle, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided data
     *
     * @param rectangle rectangle of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void draw(const Rectangle& rectangle, float angle, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided data
     *
     * @param sx start position of line
     * @param sy start position of line
     * @param ex end position of line
     * @param ey end position of line
     * @param width of line
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawLine(float sx, float sy, float ex, float ey, float width, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided data
     *
     * @param start of line
     * @param end of line
     * @param width of line
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawLine(const Vector2& start, const Vector2& end, float width, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Create and load a OpenGL shader.
     *
     * @param type Shader type to create.
     * @param code The shader code.
     * @return id for the shader
     */
    GLuint loadShader(GLenum type, const char* code);

    /**
     * Create and load a OpenGL program.
     *
     * @param program_id Shader program to create.
     * @param shaders handles to attach/use in program
     * @return id for the program
     */
    void loadProgram(unsigned int program_id, const std::vector<GLuint>& shaders);

    /**
     * Initializes OpenGL shader program with shaders code.
     */
    void initShaderPrograms();

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
