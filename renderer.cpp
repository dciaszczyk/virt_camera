#include "renderer.h"
#include <QFileInfo>
#include <QFile>// renderer.cpp
#include "renderer.h"
#include "meshLoader.h"
#include "mesh.h"
#include <QDebug>

bool Renderer::initialize()
{
    initializeOpenGLFunctions();

    if (!initShaders()) return false;
    if (!initBuffers()) return false;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initialized = true;
    return true;
}

bool Renderer::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "mesh.vert") ||
        !program.addShaderFromSourceFile(QOpenGLShader::Fragment, "mesh.frag") ||
        !program.link()) {
        qDebug() << "Shader error:" << program.log();
        return false;
    }
    return true;
}

bool Renderer::initBuffers()
{

    program.link();

    Mesh mesh = MeshLoader::load("model.txt");

    if (mesh.vertices.empty() || mesh.indices.empty()) {
        qDebug() << "Empty mesh data";
        return false;
    }

    indexCount = static_cast<GLsizei>(mesh.indices.size());

    vao.create();
    QOpenGLVertexArrayObject::Binder binder(&vao);

    vbo.create();
    vbo.bind();
    vbo.allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vec3));

    ibo.create();
    ibo.bind();
    ibo.allocate(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

    program.bind();
    program.enableAttributeArray(0);
    program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vec3));

    return true;
}

void Renderer::render(const Camera& camera, int width, int height)
{
    if (!initialized) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();
    GLint loc = glGetUniformLocation(program.programId(), "MVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, camera.getViewProjectionMatrix(width,height).m);
    program.setUniformValue("uColor", QVector3D(1.0f, 1.0f, 1.0f));

    QOpenGLVertexArrayObject::Binder binder(&vao);
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer::cleanup()
{
    vbo.destroy();
    ibo.destroy();
    vao.destroy();
    program.removeAllShaders();
    initialized = false;
}
