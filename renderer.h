#pragma once
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <vector>
#include "math3d.h"
#include "camera.h"
#include "mesh.h"

class Renderer : protected QOpenGLFunctions
{
public:
    Renderer() = default;
    bool initialize();
    void render(const Camera& camera, int width, int height);
    void cleanup();

    QOpenGLShaderProgram& getProgram() { return meshProgram; }

private:
    QOpenGLShaderProgram meshProgram;
    QOpenGLVertexArrayObject meshVao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ibo{QOpenGLBuffer::IndexBuffer};

    Mesh mesh;
    int indexCount = 0;
    bool initialized = false;
};
