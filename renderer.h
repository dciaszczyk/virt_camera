#pragma once
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include "camera.h"
#include "bsp.h"

class Renderer : protected QOpenGLFunctions
{
public:
    bool initialize();
    void render(const Camera& camera, int width, int height);
    void cleanup();

private:
    BSPTree bsp;
    bool initShaders();
    bool initBuffers();

    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ibo{QOpenGLBuffer::IndexBuffer};

    GLsizei indexCount = 0;
    bool initialized = false;
};
