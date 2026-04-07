#pragma once
#include <QWindow>
#include <QTimer>
#include "RhiWidget.h"
#include "camera.h"

class Window : public QWindow
{
public:
    Window();

protected:
    void exposeEvent(QExposeEvent*) override;
    void keyPressEvent(QKeyEvent* e) override;

private:
    void renderLoop();

    RendererRHI renderer;
    Camera camera;
    CameraController controller;

    QTimer timer;
};
