#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#include "camera.h"
#include "input.h"
#include "renderer.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    void updateCamera(float dt);
    void captureMouse();
    void releaseMouse();
    QTimer* updateTimer;
    bool keys[KeyAction::COUNT] = {false};
    bool mouseCaptured = false;
    QPoint lastMousePos;

    Camera camBsp;
    Camera camView;
    Camera activeCam;
    int activeView = 0;

    Renderer renderer;

    float mouseSensitivity = 0.05f;
    float moveSpeed = 2.0f;
    float rotSpeed = 60.0f;
};
