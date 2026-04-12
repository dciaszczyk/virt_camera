#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QPoint>
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
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    void updateCamera(float dt);
    void captureMouse();
    void releaseMouse();

private:
    QTimer* updateTimer;
    bool keys[KeyAction::COUNT] = {false};
    bool mouseCaptured = false;
    QPoint lastMousePos;

    Camera camera;

    Renderer renderer;

    float mouseSensitivity = 0.05f;
    float moveSpeed = 2.0f;
    float rotSpeed = 60.0f;
};
