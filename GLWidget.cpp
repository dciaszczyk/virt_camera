#include "GLWidget.h"
#include <QCursor>
#include <QGuiApplication>
#include <QDebug>
#include <iostream>


GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , mouseCaptured(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    camera.transform = identityMat4();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        update();
    });
    updateTimer->start(16);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    renderer.cleanup();
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    if (!renderer.initialize()) {
        qDebug() << "Failed to initialize renderer";
        return;
    }

    // GPU-safe initial camera placement
    camera.transform = identityMat4();
    camera.transform.m[14] = 10;
    camera.fov = 60.0f;
}

void GLWidget::paintGL()
{
    updateCamera(0.016f);
    renderer.render(camera, width(), height());
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape)
    {
        mouseCaptured = false;
        releaseMouse();
        releaseKeyboard();
        setCursor(Qt::ArrowCursor);
        return;
    }

    auto it = DEFAULT_KEY_BINDINGS.find(e->key());
    if (it != DEFAULT_KEY_BINDINGS.end()) {
        KeyAction action = it->second;

        switch (action) {
        case SCREENSHOT:
        {
            QImage img = grabFramebuffer();
            img.save("screenshot.png");
            break;
        }

        case SNAP_POSITION:
            camera.snapPosition();
            break;

        case SNAP_ROTATION:
            camera.snapRotation();
            break;

        default:
            keys[action] = true;
            break;
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent* e)
{
    auto it = DEFAULT_KEY_BINDINGS.find(e->key());
    if (it != DEFAULT_KEY_BINDINGS.end() &&
        it->second != SCREENSHOT &&
        it->second != SNAP_POSITION &&
        it->second != SNAP_ROTATION) {
        keys[it->second] = false;
    }
}

void GLWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton && !mouseCaptured) {
        mouseCaptured = true;
        grabMouse();
        grabKeyboard();
        setCursor(Qt::BlankCursor);
        lastMousePos = e->pos();
    }
}

void GLWidget::MouseWheelEevent(QMouseEvent* e){

}

void GLWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (!mouseCaptured)
        return;

    QPoint center = rect().center();
    QPoint delta = e->pos() - center;

    float yawDelta   = -delta.x() * mouseSensitivity;
    float pitchDelta = -delta.y() * mouseSensitivity;

    Vec3 up    = camera.getUp(camera.transform);
    Vec3 right = camera.getRight(camera.transform);

    Mat4 Ryaw   = rotationMatrix(up, yawDelta);
    Mat4 Rpitch = rotationMatrix(right, pitchDelta);

    camera.transform = camera.transform * Ryaw;
    camera.transform = camera.transform * Rpitch;

    QCursor::setPos(mapToGlobal(center));
}

inline void printCamera(const Mat4& m)
{
    std::cout
        << "POS: "
        << m.m[12] << ", "
        << m.m[13] << ", "
        << m.m[14] << "\n";

    std::cout
        << "RIGHT: "
        << m.m[0] << ", "
        << m.m[1] << ", "
        << m.m[2] << "\n";

    std::cout
        << "UP: "
        << m.m[4] << ", "
        << m.m[5] << ", "
        << m.m[6] << "\n";

    std::cout
        << "FWD: "
        << m.m[8] << ", "
        << m.m[9] << ", "
        << m.m[10] << "\n\n";
}

inline void orthonormalize(Mat4& m)
{
    Vec3 right = {m.m[0], m.m[1], m.m[2]};
    Vec3 up    = {m.m[4], m.m[5], m.m[6]};
    Vec3 fwd   = {m.m[8], m.m[9], m.m[10]};

    // Gram-Schmidt
    fwd = normalize(fwd);

    right = normalize(cross(up, fwd));
    up    = cross(fwd, right);

    // write back
    m.m[0] = right.x; m.m[1] = right.y; m.m[2] = right.z;
    m.m[4] = up.x;    m.m[5] = up.y;    m.m[6] = up.z;
    m.m[8] = fwd.x;   m.m[9] = fwd.y;   m.m[10]= fwd.z;
}

void GLWidget::updateCamera(float dt)
{
    if (!mouseCaptured)
        return;

    const float rotStep = rotSpeed * dt;
    const float moveStep = moveSpeed * dt;

    // ROTATION (keyboard)

    if (keys[ROLL_LEFT])
        camera.transform = camera.transform * rotationMatrix(camera.getForward(camera.transform), rotStep);

    if (keys[ROLL_RIGHT])
        camera.transform = camera.transform * rotationMatrix(camera.getForward(camera.transform), -rotStep);

    if (keys[ROTATE_UP])
        camera.transform = camera.transform * rotationMatrix(camera.getRight(camera.transform), rotStep);

    if (keys[ROTATE_DOWN])
        camera.transform = camera.transform * rotationMatrix(camera.getRight(camera.transform), -rotStep);

    if (keys[ROTATE_LEFT])
        camera.transform = camera.transform * rotationMatrix(camera.getUp(camera.transform), rotStep);

    if (keys[ROTATE_RIGHT])
        camera.transform = camera.transform * rotationMatrix(camera.getUp(camera.transform), -rotStep);

    // MOVEMENT (local → world via matrix)

    Vec3 localMove{0,0,0};

    if (keys[MOVE_FORWARD]) localMove.z -= moveStep;
    if (keys[MOVE_BACK])    localMove.z += moveStep;
    if (keys[MOVE_LEFT])    localMove.x -= moveStep;
    if (keys[MOVE_RIGHT])   localMove.x += moveStep;
    if (keys[MOVE_UP])      localMove.y += moveStep;
    if (keys[MOVE_DOWN])    localMove.y -= moveStep;

    Vec4 worldMove = camera.transform * Vec4{localMove.x, localMove.y, localMove.z, 0.0f};

    camera.transform.m[12] += worldMove.x;
    camera.transform.m[13] += worldMove.y;
    camera.transform.m[14] += worldMove.z;

    orthonormalize(camera.transform);

    printCamera(camera.transform);
}

void GLWidget::captureMouse()
{
    mouseCaptured = true;
    lastMousePos = mapFromGlobal(QCursor::pos());
    setCursor(Qt::BlankCursor);
    grabMouse();
    grabKeyboard();
}

void GLWidget::releaseMouse()
{
    mouseCaptured = false;
    QWidget::releaseMouse();
    QWidget::releaseKeyboard();
    setCursor(Qt::ArrowCursor);
}
