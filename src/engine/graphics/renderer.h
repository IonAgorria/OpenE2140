//
// Created by Ion Agorria on 3/11/18
//
#ifndef OPENE2140_RENDERER_H
#define OPENE2140_RENDERER_H

#include <glm/matrix.hpp>
#include "engine/core/macros.h"
#include "engine/core/macros.h"
#include "engine/graphics/color.h"
#include "engine/io/log.h"
#include "image.h"

#define PROGRAM_TEXTURE 0
#define PROGRAM_PALETTE_TEXTURE 1
#define PROGRAM_COLOR 2
#define PROGRAM_COUNT 3
#define MAX_BATCH_VERTICES 10240 //NOTE: If this exceeds ushort switch the indices to uint
#define MAX_COMPONENTS_PER_VERTICE 6

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
    GLuint programHandles[PROGRAM_COUNT] = {};

    /**
     * Vertex shader handles
     */
    GLuint programVertexHandle = 0;

    /**
     * Fragment shader handles
     */
    GLuint programFragmentHandles[PROGRAM_COUNT] = {};

    /**
     * VAO buffer handle
     */
    GLuint vaoHandle = 0;

    /**
     * VBO buffer handle
     */
    GLuint vboHandle = 0;

    /**
     * Vertices buffer to store current vertices in batch
     */
    GLfloat vertices[MAX_BATCH_VERTICES * MAX_COMPONENTS_PER_VERTICE] = {};

    /**
     * Vertices buffer written vertices count
     */
    size_t verticesCount = 0;

    /**
     * Vertices buffer index
     */
    size_t verticesIndex = 0;

    /**
     * IBO buffer handle
     */
    GLuint iboHandle = 0;

    /**
     * Indices buffer to store current indices in batch
     */
    GLushort indices[MAX_BATCH_VERTICES] = {};

    /**
     * Vertices buffer written vertices count
     */
    size_t indicesCount = 0;

    /**
     * Max texture size
     */
    int maxTextureSize = 0;

    /**
     * Last used texture for RGBA image
     */
    GLuint lastTextureImageRGBA = 0;

    /**
     * Last used texture for palette image
     */
    GLuint lastTextureImagePalette = 0;

    /**
     * Last used texture for palette
     */
    GLuint lastTexturePalette = 0;

    /**
     * Last used texture for extra palette
     */
    GLuint lastTexturePaletteExtra = 0;

    /**
     * Locations for combined uniform in shader
     */
    GLint uCombinedLocations[PROGRAM_COUNT] = {};

    /**
     * Location for uPaletteExtraOffset uniform in shader
     */
    GLint uPaletteExtraOffsetLocation = 0;

    /**
     * Location for uTextureImagePalette uniform in shader
     */
    GLint uTextureImagePaletteLocation = 0;

    /**
     * Locations for uTextureImageRGBA uniform in shader
     */
    GLint uTextureImageRGBALocation = 0;

    /**
     * Location for uTexturePalette uniform in shader
     */
    GLint uTexturePaletteLocation = 0;

    /**
     * Location for uTexturePaletteExtra uniform in shader
     */
    GLint uTexturePaletteExtraLocation = 0;

    /**
     * Current active program
     */
    int activeProgram = 0;

    /**
     * Current projection matrix
     */
    glm::mat4 projection = {};

    /**
     * Current view matrix
     */
    glm::mat4 view = {};

    /**
     * Viewport rectangle
     */
    Rectangle viewport;

    /**
     * Prepares the internal states to draw using the specified program
     *
     * @param indicesAmount amount of indices that is going to take
     * @param program wanted to use
     * @param needFlush tells that flush is necessary
     * @return if flush was done
     */
    bool prepare(size_t indicesAmount, int program, bool needFlush = false);

    /**
     * Prepares the internal states to draw the provided image
     *
     * @param indicesAmount amount of indices that is going to take
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void prepareImage(size_t indicesAmount, const Image& image, const Palette* paletteExtra);
public:
    /**
     * Flush counter
     */
    size_t flushes = 0;

    /**
     * Constructor
     */
    Renderer();

    /**
     * Destructor
     */
    ~Renderer() override;

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
     * Causes flush
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
     * Causes flush
     *
     * @param width
     * @param height
     */
    void changeCamera(int x, int y);

    /**
     * Draws the provided image
     *
     * @param x position of drawn image
     * @param y position of drawn image
     * @param width size of drawn image
     * @param height size of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawImage(float x, float y, float width, float height, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided image
     *
     * @param position of drawn image
     * @param size of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawImage(const Vector2& position, const Vector2& size, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided image centered to position and optional rotation
     *
     * @param x position of drawn image
     * @param y position of drawn image
     * @param width size of drawn image
     * @param height size of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawImageCenter(float x, float y, float width, float height, float angle, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided image centered to position and optional rotation
     *
     * @param position of drawn image
     * @param size of drawn image
     * @param angle angle of drawn image
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawImageCenter(const Vector2& position, const Vector2& size, float angle, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided line
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
     * Draws the provided line
     *
     * @param start of line
     * @param end of line
     * @param width of line
     * @param image image to draw
     * @param paletteExtra palette used to override indexed image's original palette, can be NULL
     */
    void drawLine(const Vector2& start, const Vector2& end, float width, const Image& image, const Palette* paletteExtra = nullptr);

    /**
     * Draws the provided line
     *
     * @param sx start position of line
     * @param sy start position of line
     * @param ex end position of line
     * @param ey end position of line
     * @param width of line
     * @param color of rectangle lines
     */
    void drawLine(float sx, float sy, float ex, float ey, float width, const ColorRGBA& color);

    /**
     * Draws the provided line
     *
     * @param start of line
     * @param end of line
     * @param width of line
     * @param color of rectangle lines
     */
    void drawLine(const Vector2& start, const Vector2& end, float width, const ColorRGBA& color);

    /**
     * Draws the provided rectangle
     *
     * @param x position of rectangle
     * @param y position of rectangle
     * @param w size of rectangle
     * @param h size of rectangle
     * @param width of line
     * @param color of rectangle lines
     */
    void drawRectangle(float x, float sy, float ex, float ey, float width, const ColorRGBA& color);

    /**
     * Draws the provided rectangle
     *
     * @param rectangle to draw
     * @param width of rectangle lines
     * @param color of rectangle lines
     */
    void drawRectangle(const Rectangle& rectangle, float width, const ColorRGBA& color);

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
     * @return id for the program
     */
    void loadProgram(unsigned int program_id);

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
