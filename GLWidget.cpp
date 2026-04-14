#include "GLWidget.h"
#include <QCursor>
#include <QGuiApplication>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , mouseCaptured(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

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

void GLWidget::wheelEvent(QWheelEvent* e)
{
    float delta = e->angleDelta().y() / 120.0f;

    camera.fov -= delta * 2.0f;
    camera.fov = std::clamp(camera.fov, 1.0f, 159.0f);
}

void GLWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (!mouseCaptured) return;

    QPoint center = rect().center();
    QPoint delta = QCursor::pos() - mapToGlobal(center);

    float yaw   = -delta.x() * mouseSensitivity;
    float pitch = -delta.y() * mouseSensitivity;

    camera.rotate(camera.getUp(), yaw);
    Vec3 right = camera.getRight();
    camera.rotate(right, pitch);

    QCursor::setPos(mapToGlobal(center));
}

void GLWidget::updateCamera(float dt)
{
    if (!mouseCaptured) return;

    const float rotStep = rotSpeed * dt;
    const float moveStep = moveSpeed * dt;

    Vec3 right = camera.getRight();
    Vec3 up = camera.getUp();
    Vec3 forward = camera.getForward();

    if (keys[ROTATE_LEFT])  camera.rotate(up, rotStep);
    if (keys[ROTATE_RIGHT]) camera.rotate(up, -rotStep);
    if (keys[ROTATE_UP])    camera.rotate(right, rotStep);
    if (keys[ROTATE_DOWN])  camera.rotate(right, -rotStep);
    if (keys[ROLL_LEFT])    camera.rotate(forward, +rotStep);
    if (keys[ROLL_RIGHT])   camera.rotate(forward, -rotStep);

    Vec3 move{0,0,0};
    if (keys[MOVE_FORWARD]) move.z -= moveStep;
    if (keys[MOVE_BACK])    move.z += moveStep;
    if (keys[MOVE_LEFT])    move.x -= moveStep;
    if (keys[MOVE_RIGHT])   move.x += moveStep;
    if (keys[MOVE_UP])      move.y += moveStep;
    if (keys[MOVE_DOWN])    move.y -= moveStep;

    if (dot(move, move) > 0.0f) {
        camera.move(move);
    }
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
