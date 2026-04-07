#pragma once
#include <QRhiWidget>
#include "camera.h"
#include "mesh.h"

class RhiWidget : public QRhiWidget
{
protected:
    void initialize() override;
    void render(QRhiCommandBuffer* cb) override;

private:
    // GPU resources (Qt-managed lifetime)
    QRhiBuffer* vbuf = nullptr;
    QRhiBuffer* ibuf = nullptr;
    QRhiBuffer* ubuf = nullptr;

    QRhiGraphicsPipeline* pipeline = nullptr;
    QRhiShaderResourceBindings* srb = nullptr;

    // Your data
    Camera camera;
    Mesh mesh;
};
