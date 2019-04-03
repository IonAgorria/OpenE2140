//
// Created by Ion Agorria on 3/11/18
//
#include "core/utils.h"
#include "palette.h"
#include "renderer.h"
#include "renderer_shaders.h"

Renderer::Renderer() {
    log = Log::get("Renderer");
    initShaderProgram();
    if (!error.empty()) return;
    initBuffers();
    if (!error.empty()) return;

    //Set some parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe mode

    //Enable samples
    //glEnable(GL_MULTISAMPLE);
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
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Bind the stuff
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    error = Utils::checkGLError(log);
    if (!error.empty()) return;

    //Amount of components per attrib
    const int loc0amount = 2; //Vec2
    const int loc1amount = 2; //Vec2
    const int loc2amount = 1; //Float
    const int loc3amount = 4; //Vec4
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

void Renderer::draw(float x, float y, float width, float height, float angle, Image& image, Palette* paletteExtra) {
    //Get palette
    std::shared_ptr<Palette> palette = image.getPalette();

    //Check if we need to flush the batch
    bool needFlush;
    if (palette) {
        bool textureImageChanged = !lastTextureImagePalette || lastTextureImagePalette != image.getTexture();
        bool texturePaletteChanged = !lastTexturePalette || lastTexturePalette != palette->getTexture();
        bool texturePaletteExtraChanged = paletteExtra && (!lastTexturePaletteExtra || lastTexturePaletteExtra != paletteExtra->getTexture());
        needFlush = textureImageChanged || texturePaletteChanged || texturePaletteExtraChanged;
    } else {
        needFlush = !lastTextureImageRGBA || lastTextureImageRGBA != image.getTexture();
    }
    bool bufferFull = verticesCount >= MAX_BATCH_VERTICES;
    if (needFlush || bufferFull) {
        flush();
        //Now bind the image and required palettes if any
        GLuint bindedTexture = image.bindTexture();
        if (palette) {
            lastTexturePalette = palette->bindTexture();
            lastTextureImagePalette = bindedTexture;
            if (paletteExtra) {
                lastTexturePalette = paletteExtra->bindTexture();
            }
        } else {
            lastTextureImageRGBA = bindedTexture;
        }
    }

    //Increment the vertices count
    verticesCount++;

    //Position
    vertices[verticesIndex++] = x;
    vertices[verticesIndex++] = y;
    //Size
    vertices[verticesIndex++] = width;
    vertices[verticesIndex++] = height;
    //Angle
    vertices[verticesIndex++] = angle;
    //Texture UV
    vertices[verticesIndex++] = 0;//image.u;
    vertices[verticesIndex++] = 0;//image.v;
    vertices[verticesIndex++] = 0.05;//image.u2;
    vertices[verticesIndex++] = 0.05;//image.v2;
}

bool Renderer::flush() {
    if (verticesCount > 0) {
        //Set blending
        /*
        if (blending) {
            glEnable(GL_BLEND);
            if (blendSrcFunc != -1) {
                glBlendFunc(blendSrcFunc, blendDstFunc);
            }
        } else {
            glDisable(GL_BLEND);
        }
        */

        //Bind the stuff
        glUseProgram(programHandle);
        glBindVertexArray(vaoHandle);

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
