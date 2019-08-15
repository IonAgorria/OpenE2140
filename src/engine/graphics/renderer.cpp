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
    activeProgram = PROGRAM_TEXTURE;

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

    //Set the texture units for samplers
    glUseProgram(programHandles[PROGRAM_TEXTURE]);
    glUniform1i(uTextureImageRGBALocation, 0);
    glUseProgram(programHandles[PROGRAM_PALETTE_TEXTURE]);
    glUniform1i(uTextureImagePaletteLocation, 1);
    glUniform1i(uTexturePaletteLocation, 2);
    glUniform1i(uTexturePaletteExtraLocation, 3);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

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
    for (int i = 0; i < PROGRAM_COUNT; ++i) {
        GLuint program = programHandles[i];
        if (program) {
            glDetachShader(program, programVertexHandle);
            glDetachShader(program, programFragmentHandles[i]);
            glDeleteProgram(program);
            programHandles[i] = 0;
        }
    }

    //Delete shaders
    if (programVertexHandle) {
        glDeleteShader(programVertexHandle);
        programVertexHandle = 0;
    }
    for (auto& programFragmentHandle : programFragmentHandles) {
        if (programFragmentHandle) {
            glDeleteShader(programFragmentHandle);
            programFragmentHandle = 0;
        }
    }

    //Delete other stuff
    if (iboHandle) {
        glDeleteBuffers(1, &iboHandle);
        iboHandle = 0;
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

void Renderer::loadProgram(unsigned int program_id) {
    //Create program
    GLuint program = glCreateProgram();
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Attach shaders
    glAttachShader(program, programVertexHandle);
    glAttachShader(program, programFragmentHandles[program_id]);
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
    log->debug("Initializing shader programs");

    //Load shaders
    programVertexHandle = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    if (!error.empty()) return;
    programFragmentHandles[PROGRAM_TEXTURE] = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_TEXTURE);
    if (!error.empty()) return;
    programFragmentHandles[PROGRAM_PALETTE_TEXTURE] = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PALETTE_TEXTURE);
    if (!error.empty()) return;
    programFragmentHandles[PROGRAM_COLOR] = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_COLOR);
    if (!error.empty()) return;

    //Create programs
    loadProgram(PROGRAM_TEXTURE);
    if (!error.empty()) return;
    loadProgram(PROGRAM_PALETTE_TEXTURE);
    if (!error.empty()) return;
    loadProgram(PROGRAM_COLOR);
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
    uTextureImageRGBALocation = glGetUniformLocation(programHandles[PROGRAM_TEXTURE], "uTextureImageRGBA");
    GLint programPaletteTexture = programHandles[PROGRAM_PALETTE_TEXTURE];
    uPaletteExtraOffsetLocation = glGetUniformLocation(programPaletteTexture, "uPaletteExtraOffset");
    uTextureImagePaletteLocation = glGetUniformLocation(programPaletteTexture, "uTextureImagePalette");
    uTexturePaletteLocation = glGetUniformLocation(programPaletteTexture, "uTexturePalette");
    uTexturePaletteExtraLocation = glGetUniformLocation(programPaletteTexture, "uTexturePaletteExtra");

    error = Utils::checkGLError(log);
    if (error.empty() && (uTextureImageRGBALocation < 0 || uTextureImagePaletteLocation < 0
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
    glGenBuffers(1, &iboHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Bind the stuff
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Amount of components per attrib
    const int loc0amount = 2; //Vec2
    const int loc1amount = 4; //Vec4
    //Total bytes taken by each attrib
    const size_t loc0bytes = loc0amount * sizeof(GLfloat);
    const size_t loc1bytes = loc1amount * sizeof(GLfloat);
    //Offset for attrib in relation to previous attribute end
    const GLvoid* loc0offset = 0;
    const GLvoid* loc1offset = (GLvoid*) (loc0bytes);
    //All attributes bytes count in a entire vertex
    const int stride = loc0bytes + loc1bytes;
    //Setup all the vertex attributes data on this vertex array
    glVertexAttribPointer(0, loc0amount, GL_FLOAT, GL_FALSE, stride, loc0offset);
    glVertexAttribPointer(1, loc1amount, GL_FLOAT, GL_FALSE, stride, loc1offset);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Enable them
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;
}

bool Renderer::prepare(size_t indicesAmount, int program, bool needFlush) {
    needFlush |= program != activeProgram;
    needFlush |= indicesCount + indicesAmount >= MAX_BATCH_VERTICES;

    //Check if we need to flush the batch
    if (needFlush) {
        flush();

        //Set the active program
        activeProgram = program;
        glUseProgram(programHandles[activeProgram]);
        glBindVertexArray(vaoHandle);
    }
    return needFlush;
}

void Renderer::prepareImage(size_t indicesAmount, const Image& image, const Palette* paletteExtra) {
    //Get palette
    const std::shared_ptr<Palette>& palette = image.getPalette();

    bool needFlush;
    int requiredProgram;
    if (palette) {
        //Palette image
        requiredProgram = PROGRAM_PALETTE_TEXTURE;
        bool textureImageChanged = !lastTextureImagePalette || lastTextureImagePalette != image.getTexture();
        bool texturePaletteChanged = !lastTexturePalette || lastTexturePalette != palette->getTexture();
        bool texturePaletteExtraChanged = paletteExtra && (!lastTexturePaletteExtra || lastTexturePaletteExtra != paletteExtra->getTexture());
        needFlush = textureImageChanged || texturePaletteChanged || texturePaletteExtraChanged;
    } else {
        //RGBA image
        requiredProgram = PROGRAM_TEXTURE;
        needFlush = !lastTextureImageRGBA || lastTextureImageRGBA != image.getTexture();
    }
    needFlush = prepare(indicesAmount, requiredProgram, needFlush);

    //Check if it was flushed
    if (needFlush) {
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

void Renderer::drawImage(float x, float y, float width, float height, float angle, const Image& image, const Palette* paletteExtra) {
    prepareImage(6, image, paletteExtra);

    //Add the indices
    indices[indicesCount++] = verticesCount;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 3;

    //Increment the vertices count
    verticesCount += 4;

    //Get the half of size
    width /= 2;
    height /= 2;

    //Position
    if (angle == 0) {
        //Non rotated rectangle mode

        //Bottom left
        vertices[verticesIndex++] = x - width;
        vertices[verticesIndex++] = y - height;
        vertices[verticesIndex++] = image.u;
        vertices[verticesIndex++] = image.v;
        vertices[verticesIndex++] = 0;
        vertices[verticesIndex++] = 0;

        //Bottom right
        vertices[verticesIndex++] = x + width;
        vertices[verticesIndex++] = y - height;
        vertices[verticesIndex++] = image.u2;
        vertices[verticesIndex++] = image.v;
        vertices[verticesIndex++] = 1;
        vertices[verticesIndex++] = 0;

        //Top left
        vertices[verticesIndex++] = x - width;
        vertices[verticesIndex++] = y + height;
        vertices[verticesIndex++] = image.u;
        vertices[verticesIndex++] = image.v2;
        vertices[verticesIndex++] = 0;
        vertices[verticesIndex++] = 1;

        //Top right
        vertices[verticesIndex++] = x + width;
        vertices[verticesIndex++] = y + height;
        vertices[verticesIndex++] = image.u2;
        vertices[verticesIndex++] = image.v2;
        vertices[verticesIndex++] = 1;
        vertices[verticesIndex++] = 1;
    } else {
        //Rotated rectangle mode
        float rs = sin(angle);
        float rc = cos(angle);

        //Bottom left
        vertices[verticesIndex++] = x - ((rc * -width) - (rs * -height));
        vertices[verticesIndex++] = y - ((rs * -width) + (rc * -height));
        vertices[verticesIndex++] = image.u;
        vertices[verticesIndex++] = image.v;
        vertices[verticesIndex++] = 0;
        vertices[verticesIndex++] = 0;

        //Bottom right
        vertices[verticesIndex++] = x + ((rc *  width) - (rs * -height));
        vertices[verticesIndex++] = y - ((rs *  width) + (rc * -height));
        vertices[verticesIndex++] = image.u2;
        vertices[verticesIndex++] = image.v;
        vertices[verticesIndex++] = 1;
        vertices[verticesIndex++] = 0;

        //Top left
        vertices[verticesIndex++] = x - ((rc * -width) - (rs *  height));
        vertices[verticesIndex++] = y + ((rs * -width) + (rc *  height));
        vertices[verticesIndex++] = image.u;
        vertices[verticesIndex++] = image.v2;
        vertices[verticesIndex++] = 0;
        vertices[verticesIndex++] = 1;

        //Top right
        vertices[verticesIndex++] = x + ((rc *  width) - (rs * height));
        vertices[verticesIndex++] = y + ((rs *  width) + (rc * height));
        vertices[verticesIndex++] = image.u2;
        vertices[verticesIndex++] = image.v2;
        vertices[verticesIndex++] = 1;
        vertices[verticesIndex++] = 1;
    }
}

void Renderer::drawImage(const Vector2& position, const Vector2& size, float angle, const Image& image, const Palette* paletteExtra) {
    drawImage(
            static_cast<float>(position.x),
            static_cast<float>(position.y),
            static_cast<float>(size.x),
            static_cast<float>(size.y),
            angle, image, paletteExtra
    );
}

void Renderer::drawLine(float sx, float sy, float ex, float ey, float width, const ColorRGBA& color) {
    prepare(6, PROGRAM_COLOR);

    //Add the indices
    indices[indicesCount++] = verticesCount;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 3;

    //Increment the vertices count
    verticesCount += 4;

    //Calculate stuff
    width /= 2; //Half of width is used in each side
    float angle = static_cast<float>(std::atan2(ey - sy, ex - sx) + M_PI_2);
    float rc = std::cos(angle) * width;
    float rs = std::sin(angle) * width;
    float r = static_cast<float>(color.r) / 255.0f;
    float g = static_cast<float>(color.g) / 255.0f;
    float b = static_cast<float>(color.b) / 255.0f;
    float a = static_cast<float>(color.a) / 255.0f;

    //Start left
    vertices[verticesIndex++] = sx - rc;
    vertices[verticesIndex++] = sy - rs;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //Start right
    vertices[verticesIndex++] = sx + rc;
    vertices[verticesIndex++] = sy + rs;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //End left
    vertices[verticesIndex++] = ex - rc;
    vertices[verticesIndex++] = ey - rs;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //End right
    vertices[verticesIndex++] = ex + rc;
    vertices[verticesIndex++] = ey + rs;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;
}

void Renderer::drawLine(const Vector2& start, const Vector2& end, float width, const ColorRGBA& color) {
    drawLine(
            static_cast<float>(start.x),
            static_cast<float>(start.y),
            static_cast<float>(end.x),
            static_cast<float>(end.y),
            width, color
    );
}

void Renderer::drawRectangle(float x, float y, float w, float h, float width, const ColorRGBA& color) {
    prepare(24, PROGRAM_COLOR);

    //Calculate stuff
    float r = static_cast<float>(color.r) / 255.0f;
    float g = static_cast<float>(color.g) / 255.0f;
    float b = static_cast<float>(color.b) / 255.0f;
    float a = static_cast<float>(color.a) / 255.0f;

    //Bottom
    indices[indicesCount++] = verticesCount;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 3;

    //Left
    indices[indicesCount++] = verticesCount;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 6;
    indices[indicesCount++] = verticesCount + 6;
    indices[indicesCount++] = verticesCount + 2;
    indices[indicesCount++] = verticesCount + 4;

    //Right
    indices[indicesCount++] = verticesCount + 3;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 5;
    indices[indicesCount++] = verticesCount + 5;
    indices[indicesCount++] = verticesCount + 1;
    indices[indicesCount++] = verticesCount + 7;

    //Top
    indices[indicesCount++] = verticesCount + 4;
    indices[indicesCount++] = verticesCount + 5;
    indices[indicesCount++] = verticesCount + 6;
    indices[indicesCount++] = verticesCount + 6;
    indices[indicesCount++] = verticesCount + 5;
    indices[indicesCount++] = verticesCount + 7;

    //0 Outside bottom left
    vertices[verticesIndex++] = x;
    vertices[verticesIndex++] = y;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //1 Outside bottom right
    vertices[verticesIndex++] = x + w;
    vertices[verticesIndex++] = y;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //2 Inner bottom left
    vertices[verticesIndex++] = x + width;
    vertices[verticesIndex++] = y + width;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //3 Inner bottom right
    vertices[verticesIndex++] = x + w - width;
    vertices[verticesIndex++] = y + width;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //4 Inner bottom left
    vertices[verticesIndex++] = x + width;
    vertices[verticesIndex++] = y + h - width;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //5 Inner bottom right
    vertices[verticesIndex++] = x + w - width;
    vertices[verticesIndex++] = y + h - width;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //6 Outside top left
    vertices[verticesIndex++] = x;
    vertices[verticesIndex++] = y + h;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //7 Outside top left
    vertices[verticesIndex++] = x + w;
    vertices[verticesIndex++] = y + h;
    vertices[verticesIndex++] = r;
    vertices[verticesIndex++] = g;
    vertices[verticesIndex++] = b;
    vertices[verticesIndex++] = a;

    //Increment the vertices count
    verticesCount += 8;
}

void Renderer::drawRectangle(const Rectangle& rectangle, float width, const ColorRGBA& color) {
    drawRectangle(
            static_cast<float>(rectangle.x),
            static_cast<float>(rectangle.y),
            static_cast<float>(rectangle.w),
            static_cast<float>(rectangle.h),
            width, color
    );
}

bool Renderer::flush() {
    if (verticesCount > 0) {
        //Load combined matrix before drawing
        glm::mat4 combined = projection * view;
        glUniformMatrix4fv(uCombinedLocations[activeProgram], 1, GL_FALSE, glm::value_ptr(combined));

        //Load data
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesIndex, vertices, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indicesCount, indices, GL_DYNAMIC_DRAW);

        //Draw it
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);

        //Check any error
        error = Utils::checkGLError(log);
        if (!error.empty()) return false;

        //Reset the counters
        verticesIndex = 0;
        verticesCount = 0;
        indicesCount = 0;
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
