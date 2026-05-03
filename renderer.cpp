#include "renderer.h"
#include <QFileInfo>
#include <QFile>
#include "renderer.h"
#include "meshLoader.h"
#include "mesh.h"
#include <QDebug>

bool Renderer::initialize()
{
    initializeOpenGLFunctions();

    if (!initShaders()) return false;
    if (!initBuffers()) return false;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
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

    std::vector<Triangle> triangles;

    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        Triangle t;
        t.v0 = mesh.vertices[mesh.indices[i]];
        t.v1 = mesh.vertices[mesh.indices[i+1]];
        t.v2 = mesh.vertices[mesh.indices[i+2]];
        t.computePlane();

        triangles.push_back(t);
    }

    bsp.build(triangles);

    return true;
}

void Renderer::render(const Camera& camera, int width, int height)
{
    if (!initialized) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();
    GLint loc = glGetUniformLocation(program.programId(), "MVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, camera.getViewProjectionMatrix(width,height).m);

    auto sortedTriangles = bsp.getSorted(camera.position);

    QOpenGLVertexArrayObject::Binder binder(&vao);
    std::vector<Vec3> buffer;

    for (const auto& t : sortedTriangles)
    {
        buffer.push_back(t.v0);
        buffer.push_back(t.v1);
        buffer.push_back(t.v2);
    }

    vbo.bind();
    vbo.allocate(buffer.data(), buffer.size() * sizeof(Vec3));

    program.setUniformValue("uCameraPos",
                            QVector3D(camera.position.x, camera.position.y, camera.position.z));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, buffer.size());

}

void Renderer::cleanup()
{
    vbo.destroy();
    ibo.destroy();
    vao.destroy();
    program.removeAllShaders();
    initialized = false;
}
