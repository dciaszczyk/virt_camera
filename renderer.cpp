#include "renderer.h"
#include "mesh_loader.h"
#include <QFileInfo>
#include <QFile>

bool Renderer::initialize()
{
    initializeOpenGLFunctions();

    if (!QFile::exists("mesh.vert") || !QFile::exists("mesh.frag")) {
        return false;
    }

    if (!meshProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "mesh.vert")) {
        return false;
    }
    if (!meshProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "mesh.frag")) {
        return false;
    }
    if (!meshProgram.link()) {
        return false;
    }

    mesh = MeshLoader::load("model.txt");

    indexCount = mesh.indices.size();

    if (indexCount == 0) {
        return false;
    }

    meshVao.create();
    if (!meshVao.isCreated()) {
        return false;
    }
    meshVao.bind();

    vbo.create();
    if (!vbo.isCreated()) {
        return false;
    }
    vbo.bind();
    vbo.allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vec3));

    ibo.create();
    if (!ibo.isCreated()) {
        return false;
    }
    ibo.bind();
    ibo.allocate(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));

    meshProgram.bind();
    meshProgram.enableAttributeArray(0);
    meshProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vec3));

    meshVao.release();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // opaque black

    initialized = true;
    return true;
}

QMatrix4x4 toQMatrix(const Mat4& m)
{
    return QMatrix4x4(
        m.m[0], m.m[4], m.m[8],  m.m[12],
        m.m[1], m.m[5], m.m[9],  m.m[13],
        m.m[2], m.m[6], m.m[10], m.m[14],
        m.m[3], m.m[7], m.m[11], m.m[15]
        );
}

void Renderer::render(const Camera& camera, int width, int height)
{
    if (!initialized)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 proj;
    proj.perspective(60.0f, float(width)/height, 0.1f, 100.0f);

    QMatrix4x4 view = toQMatrix(camera.getViewMatrix());

    QMatrix4x4 vp = proj * view;

    meshProgram.bind();
    meshProgram.setUniformValue("MVP", vp);
    meshProgram.setUniformValue("uColor", QVector3D(1.0f, 1.0f, 1.0f));

    meshVao.bind();

    if (!ibo.isCreated()) {
        meshVao.release();
        return;
    }

    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);

    GLenum error = glGetError();
    meshVao.release();
}

void Renderer::cleanup()
{
    if (initialized) {
        vbo.destroy();
        ibo.destroy();
        meshVao.destroy();
        meshProgram.removeAllShaders();
        initialized = false;
    }
}
