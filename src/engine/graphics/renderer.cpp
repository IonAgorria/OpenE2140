//
// Created by Ion Agorria on 3/11/18
//
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/core/utils.h"
#include "palette.h"
#include "renderer.h"
#include "renderer_shaders.h"

Renderer::Renderer() {
    log = Log::get("Renderer");
    activeProgram = PROGRAM_RECTANGLE_TEXTURE;

    initShaderPrograms();
    if (!error.empty()) return;
    initBuffers();
    if (!error.empty()) return;

    //Get the max texture size allowed
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    log->debug("GL_MAX_TEXTURE_SIZE: {0}", maxTextureSize);

#if GAME_IS_MACOS
    //MacOS doesn't draw anything if max texture size is used so we reduce it by half
    if (maxTextureSize / 2 > MINIMUM_TEXTURE_SIZE) {
        log->debug("Workaround: Reduced max texture size by half");
        maxTextureSize /= 2;
    }
#endif

#if MAXIMUM_TEXTURE_SIZE
    //Clamp the texture size so the packing doesn't use too much memory
    if (maxTextureSize > MAXIMUM_TEXTURE_SIZE) {
        maxTextureSize = MAXIMUM_TEXTURE_SIZE;
    }
#endif

    //Check the texture size is not below minimum
    if (maxTextureSize < MINIMUM_TEXTURE_SIZE) {
        error = "Maximum texture size is too small: " + std::to_string(maxTextureSize);
        return;
    }

    //Set some parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe mode

    //Enable samples
    //glEnable(GL_MULTISAMPLE);

    //Set the texture units for samplers
    glUseProgram(programHandles[PROGRAM_RECTANGLE_TEXTURE]);
    glUniform1i(uTextureImageRGBALocations[PROGRAM_RECTANGLE_TEXTURE], 0);
    glUseProgram(programHandles[PROGRAM_LINE_TEXTURE]);
    glUniform1i(uTextureImageRGBALocations[PROGRAM_LINE_TEXTURE], 0);
    glUseProgram(programHandles[PROGRAM_RECTANGLE_PALETTE_TEXTURE]);
    glUniform1i(uTextureImagePaletteLocation, 1);
    glUniform1i(uTexturePaletteLocation, 2);
    glUniform1i(uTexturePaletteExtraLocation, 3);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Bind the stuff
    glUseProgram(programHandles[activeProgram]);
    glBindVertexArray(vaoHandle);

    //Set blending func
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Set initial camera
    changeCamera(0, 0);
}

Renderer::~Renderer() {
    log->debug("Closing");

    //Delete programs
    glUseProgram(0);
    GLuint program = programHandles[PROGRAM_RECTANGLE_TEXTURE];
    if (program) {
        glDetachShader(program, programVertexHandle);
        glDetachShader(program, programGeometryRectangleHandle);
        glDetachShader(program, programFragmentTextureHandle);
        glDeleteProgram(program);
        programHandles[PROGRAM_RECTANGLE_TEXTURE] = 0;
    }
    program = programHandles[PROGRAM_RECTANGLE_PALETTE_TEXTURE];
    if (program) {
        glDetachShader(program, programVertexHandle);
        glDetachShader(program, programGeometryRectangleHandle);
        glDetachShader(program, programFragmentPaletteTextureHandle);
        glDeleteProgram(program);
        programHandles[PROGRAM_RECTANGLE_PALETTE_TEXTURE] = 0;
    }
    program = programHandles[PROGRAM_LINE_TEXTURE];
    if (program) {
        glDetachShader(program, programVertexHandle);
        glDetachShader(program, programGeometryLineHandle);
        glDetachShader(program, programFragmentTextureHandle);
        glDeleteProgram(program);
        programHandles[PROGRAM_LINE_TEXTURE] = 0;
    }

    //Delete shaders
    if (programVertexHandle) {
        glDeleteShader(programVertexHandle);
        programVertexHandle = 0;
    }
    if (programGeometryRectangleHandle) {
        glDeleteShader(programGeometryRectangleHandle);
        programGeometryRectangleHandle = 0;
    }
    if (programGeometryLineHandle) {
        glDeleteShader(programGeometryLineHandle);
        programGeometryLineHandle = 0;
    }
    if (programFragmentTextureHandle) {
        glDeleteShader(programFragmentTextureHandle);
        programFragmentTextureHandle = 0;
    }
    if (programFragmentPaletteTextureHandle) {
        glDeleteShader(programFragmentPaletteTextureHandle);
        programFragmentPaletteTextureHandle = 0;
    }

    //Delete other stuff
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
    std::string infoLog;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (0 < logSize) {
        std::vector<GLchar> buffer;
        buffer.resize(static_cast<size_t>(logSize));
        glGetShaderInfoLog(shader, logSize, &logSize, buffer.data());
        infoLog = buffer.data();
    }
    if (!infoLog.empty()) {
        if (Utils::toLower(infoLog).find("error") != std::string::npos) {
            error = "Shader " + std::to_string(shader) + " type " + std::to_string(type) + " compile error: " + infoLog;
            return 0;
        } else {
            log->debug("Shader type {0} compile log: {1}", type, infoLog);
        }
    }

    return shader;
}

void Renderer::loadProgram(unsigned int program_id, const std::vector<GLuint>& shaders) {
    //Create program
    GLuint program = glCreateProgram();
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Attach shaders
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Link the program
    glLinkProgram(program);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Get info log
    int logSize;
    std::string infoLog;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    if (0 < logSize) {
        std::vector<GLchar> buffer;
        buffer.resize(static_cast<size_t>(logSize));
        glGetProgramInfoLog(program, logSize, &logSize, buffer.data());
        infoLog = buffer.data();
    }
    if (!infoLog.empty()) {
        if (Utils::toLower(infoLog).find("error") != std::string::npos) {
            error = "Program " + std::to_string(program) + " compile error: " + infoLog;
            return;
        } else {
            log->debug("Program {0} compile log: {1}", program, infoLog);
        }
    }

    //Store program
    programHandles[program_id] = program;
}

void Renderer::initShaderPrograms() {
    log->debug("Initializing shader program");
    //Load shaders
    programVertexHandle = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    if (!error.empty()) return;
    programGeometryRectangleHandle = loadShader(GL_GEOMETRY_SHADER, GEOMETRY_SHADER_RECTANGLE);
    if (!error.empty()) return;
    programGeometryLineHandle = loadShader(GL_GEOMETRY_SHADER, GEOMETRY_SHADER_RECTANGLE); //TODO
    if (!error.empty()) return;
    programFragmentTextureHandle = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_TEXTURE);
    if (!error.empty()) return;
    programFragmentPaletteTextureHandle = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PALETTE_TEXTURE);
    if (!error.empty()) return;

    //Create programs
    loadProgram(PROGRAM_RECTANGLE_TEXTURE, {
            programVertexHandle, programGeometryRectangleHandle, programFragmentTextureHandle
    });
    if (!error.empty()) return;
    loadProgram(PROGRAM_RECTANGLE_PALETTE_TEXTURE, {
            programVertexHandle, programGeometryRectangleHandle, programFragmentPaletteTextureHandle
    });
    if (!error.empty()) return;
    loadProgram(PROGRAM_LINE_TEXTURE, {
            programVertexHandle, programGeometryLineHandle, programFragmentTextureHandle
    });
    if (!error.empty()) return;

    //Get the locations
    for (int i = 0; i < PROGRAM_COUNT; ++i) {
        GLint location = glGetUniformLocation(programHandles[i], "uCombined");
        error = Utils::checkGLError(log);
        if (error.empty() && location < 0) {
            std::string text = "Uniform location not found in shader " + std::to_string(i);
            if (Utils::isFlag(FLAG_DEBUG)) {
                log->warn(text);
            } else {
                error = text;
            }
        }
        if (!error.empty()) return;
        uCombinedLocations[i] = location;
    }
    uTextureImageRGBALocations[PROGRAM_RECTANGLE_TEXTURE] = glGetUniformLocation(programHandles[PROGRAM_RECTANGLE_TEXTURE], "uTextureImageRGBA");
    uTextureImageRGBALocations[PROGRAM_LINE_TEXTURE] = glGetUniformLocation(programHandles[PROGRAM_LINE_TEXTURE], "uTextureImageRGBA");
    GLint programPaletteTexture = programHandles[PROGRAM_RECTANGLE_PALETTE_TEXTURE];
    uPaletteExtraOffsetLocation = glGetUniformLocation(programPaletteTexture, "uPaletteExtraOffset");
    uTextureImagePaletteLocation = glGetUniformLocation(programPaletteTexture, "uTextureImagePalette");
    uTexturePaletteLocation = glGetUniformLocation(programPaletteTexture, "uTexturePalette");
    uTexturePaletteExtraLocation = glGetUniformLocation(programPaletteTexture, "uTexturePaletteExtra");

    error = Utils::checkGLError(log);
    if (error.empty() && (uTextureImageRGBALocations[PROGRAM_RECTANGLE_TEXTURE] < 0
                      || uTextureImageRGBALocations[PROGRAM_LINE_TEXTURE] < 0
                      || uTextureImagePaletteLocation < 0
                      || uTexturePaletteLocation < 0 || uTexturePaletteExtraLocation < 0
    )) {
        std::string text = "Uniform location not found in shaders";
        if (Utils::isFlag(FLAG_DEBUG)) {
            log->warn(text);
        } else {
            error = text;
        }
    }
    if (!error.empty()) return;
}

void Renderer::initBuffers() {
    //Generate buffers
    glGenVertexArrays(1, &vaoHandle);
    glGenBuffers(1, &vboHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Bind the stuff
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Amount of components per attrib
    const int loc0amount = 2; //Vec2
    const int loc1amount = 4; //Vec4
    const int loc2amount = 2; //Vec2
    const int loc3amount = 1; //Float
    //Total bytes taken by each attrib
    const size_t loc0bytes = loc0amount * sizeof(GLfloat);
    const size_t loc1bytes = loc1amount * sizeof(GLfloat);
    const size_t loc2bytes = loc2amount * sizeof(GLfloat);
    const size_t loc3bytes = loc3amount * sizeof(GLfloat);
    //Offset for attrib in relation to previous attribute end
    const GLvoid* loc0offset = 0;
    const GLvoid* loc1offset = (GLvoid*) (loc0bytes);
    const GLvoid* loc2offset = (GLvoid*) (loc0bytes + loc1bytes);
    const GLvoid* loc3offset = (GLvoid*) (loc0bytes + loc1bytes + loc2bytes);
    //All attributes bytes count in a entire vertex
    const int stride = loc0bytes + loc1bytes + loc2bytes + loc3bytes;
    //Setup all the vertex attributes data on this vertex array
    glVertexAttribPointer(0, loc0amount, GL_FLOAT, GL_FALSE, stride, loc0offset);
    glVertexAttribPointer(1, loc1amount, GL_FLOAT, GL_FALSE, stride, loc1offset);
    glVertexAttribPointer(2, loc2amount, GL_FLOAT, GL_FALSE, stride, loc2offset);
    glVertexAttribPointer(3, loc3amount, GL_FLOAT, GL_FALSE, stride, loc3offset);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Enable them
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;
}

void Renderer::prepare(const Image& image, const Palette* paletteExtra, bool line) {
    //Get palette
    const std::shared_ptr<Palette>& palette = image.getPalette();

    bool needFlush;
    int requiredProgram;
    if (palette) {
        //Palette image
        requiredProgram = PROGRAM_RECTANGLE_PALETTE_TEXTURE;
        bool textureImageChanged = !lastTextureImagePalette || lastTextureImagePalette != image.getTexture();
        bool texturePaletteChanged = !lastTexturePalette || lastTexturePalette != palette->getTexture();
        bool texturePaletteExtraChanged = paletteExtra && (!lastTexturePaletteExtra || lastTexturePaletteExtra != paletteExtra->getTexture());
        needFlush = textureImageChanged || texturePaletteChanged || texturePaletteExtraChanged;
    } else {
        //RGBA image
        requiredProgram = line ? PROGRAM_LINE_TEXTURE : PROGRAM_RECTANGLE_TEXTURE;
        needFlush = !lastTextureImageRGBA || lastTextureImageRGBA != image.getTexture();
    }
    needFlush |= verticesCount >= MAX_BATCH_VERTICES;
    needFlush |= requiredProgram != activeProgram;

    //Check if we need to flush the batch
    if (needFlush) {
        flush();

        //Set the active program
        activeProgram = requiredProgram;
        glUseProgram(programHandles[activeProgram]);

        //Now bind the image and required palettes if any
        GLuint bindedTexture = image.bindTexture();
        if (palette) {
            lastTexturePalette = palette->bindTexture();
            lastTextureImagePalette = bindedTexture;

            //Set the extra palette and offset (disabled if negative)
            unsigned int paletteExtraOffset = -1;
            if (paletteExtra) {
                lastTexturePalette = paletteExtra->bindTexture();
                paletteExtraOffset = 0x100 - paletteExtra->length();
            }
            glUniform1i(uPaletteExtraOffsetLocation, paletteExtraOffset);
        } else {
            lastTextureImageRGBA = bindedTexture;
        }
    }
}

void Renderer::draw(float x, float y, float width, float height, float angle, const Image& image, const Palette* paletteExtra) {
    prepare(image, paletteExtra, false);

    //Increment the vertices count
    verticesCount++;

    //Position
    vertices[verticesIndex++] = x;
    vertices[verticesIndex++] = y;
    //Texture UV
    vertices[verticesIndex++] = image.u;
    vertices[verticesIndex++] = image.v;
    vertices[verticesIndex++] = image.u2;
    vertices[verticesIndex++] = image.v2;
    //Size
    vertices[verticesIndex++] = width / 2.0f;
    vertices[verticesIndex++] = height / 2.0f;
    //Angle
    vertices[verticesIndex++] = angle;
}

void Renderer::draw(const Vector2& position, const Vector2& size, float angle, const Image& image, const Palette* paletteExtra) {
    prepare(image, paletteExtra, false);

    //Increment the vertices count
    verticesCount++;

    //Position
    vertices[verticesIndex++] = position.x;
    vertices[verticesIndex++] = position.y;
    //Texture UV
    vertices[verticesIndex++] = image.u;
    vertices[verticesIndex++] = image.v;
    vertices[verticesIndex++] = image.u2;
    vertices[verticesIndex++] = image.v2;
    //Size
    vertices[verticesIndex++] = static_cast<float>(size.x) / 2.0f;
    vertices[verticesIndex++] = static_cast<float>(size.y) / 2.0f;
    //Angle
    vertices[verticesIndex++] = angle;
}

void Renderer::draw(const Rectangle& rectangle, float angle, const Image& image, const Palette* paletteExtra) {
    prepare(image, paletteExtra, false);

    //Increment the vertices count
    verticesCount++;
    //Get size
    float w = static_cast<float>(rectangle.w) / 2.0f;
    float h = static_cast<float>(rectangle.h) / 2.0f;

    //Position
    vertices[verticesIndex++] = static_cast<float>(rectangle.x) + w;
    vertices[verticesIndex++] = static_cast<float>(rectangle.y) + h;
    //Texture UV
    vertices[verticesIndex++] = image.u;
    vertices[verticesIndex++] = image.v;
    vertices[verticesIndex++] = image.u2;
    vertices[verticesIndex++] = image.v2;
    //Size
    vertices[verticesIndex++] = w;
    vertices[verticesIndex++] = h;
    //Angle
    vertices[verticesIndex++] = angle;
}

void Renderer::drawLine(float sx, float sy, float ex, float ey, float width, const Image& image, const Palette* paletteExtra) {
    prepare(image, paletteExtra, true);

    //Increment the vertices count
    verticesCount++;

    //Start position
    vertices[verticesIndex++] = sx;
    vertices[verticesIndex++] = sy;
    //Texture UV
    vertices[verticesIndex++] = image.u;
    vertices[verticesIndex++] = image.v;
    vertices[verticesIndex++] = image.u2;
    vertices[verticesIndex++] = image.v2;
    //End position
    vertices[verticesIndex++] = ex;
    vertices[verticesIndex++] = ey;
    //Width
    vertices[verticesIndex++] = width;
}

void Renderer::drawLine(const Vector2& start, const Vector2& end, float width, const Image& image, const Palette* paletteExtra) {
    prepare(image, paletteExtra, true);

    //Increment the vertices count
    verticesCount++;

    //Start position
    vertices[verticesIndex++] = start.x;
    vertices[verticesIndex++] = start.y;
    //Texture UV
    vertices[verticesIndex++] = image.u;
    vertices[verticesIndex++] = image.v;
    vertices[verticesIndex++] = image.u2;
    vertices[verticesIndex++] = image.v2;
    //End position
    vertices[verticesIndex++] = end.x;
    vertices[verticesIndex++] = end.y;
    //Width
    vertices[verticesIndex++] = width;
}

bool Renderer::flush() {
    if (verticesCount > 0) {
        //Load combined matrix before drawing
        glm::mat4 combined = projection * view;
        glUniformMatrix4fv(uCombinedLocations[activeProgram], 1, GL_FALSE, glm::value_ptr(combined));

        //Load data
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesIndex, vertices, GL_DYNAMIC_DRAW);

        //Draw it
        glDrawArrays(GL_POINTS, 0, verticesCount);

        //Check any error
        error = Utils::checkGLError(log);
        if (!error.empty()) return false;

        //Reset the counters
        verticesIndex = 0;
        verticesCount = 0;
    }

    return true;
}

void Renderer::changeViewport(int x, int y, int width, int height) {
    viewport.set(x, y, width, height);
    glViewport(x, y, width, height);
    projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f, 3.0f);
}

const Rectangle& Renderer::getViewport() const {
    return viewport;
}

void Renderer::changeCamera(int x, int y) {
    view = glm::translate(glm::mat4(1.0f), glm::vec3(-x, -y, -2));
}

unsigned int Renderer::getMaxTextureSize() {
    return static_cast<unsigned int>(maxTextureSize);
}
