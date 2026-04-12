void RhiWidget::initialize()
{
    QRhi* r = rhi();

    // --- Buffers ---
    vbuf = r->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, SIZE);
    vbuf->create();

    ibuf = r->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::IndexBuffer, SIZE);
    ibuf->create();

    ubuf = r->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 64);
    ubuf->create();

    // --- Shader bindings ---
    srb = r->newShaderResourceBindings();
    srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(
            0,
            QRhiShaderResourceBinding::VertexStage,
            ubuf)
    });
    srb->create();

    // --- Pipeline ---
    pipeline = r->newGraphicsPipeline();
    pipeline->setTopology(QRhiGraphicsPipeline::Lines);

    // shaders (.qsb required)
    pipeline->setShaderStages({
        { QRhiShaderStage::Vertex, loadVertexShader() },
        { QRhiShaderStage::Fragment, loadFragmentShader() }
    });

    pipeline->setShaderResourceBindings(srb);
    pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());

    pipeline->create();

    // --- Load mesh (your code) ---
    mesh = MeshLoader::loadFromFile("model.txt");

    // Upload mesh → buffers (you define how)
}

void RhiWidget::render(QRhiCommandBuffer* cb)
{
    // --- YOUR CAMERA + MATH ---
    auto view = camera.getViewMatrix();
    auto proj = camera.getProjectionMatrix();
    auto model = mesh.getModelMatrix();

    auto mvp = proj * view * model;

    // --- Upload MVP to uniform buffer ---
    // (write raw bytes of Mat4 into ubuf)

    // --- Issue draw ---
    cb->setGraphicsPipeline(pipeline);
    cb->setShaderResources();

    const QRhiCommandBuffer::VertexInput vInput(vbuf, 0);

    cb->setVertexInput(
        0, 1, &vInput,
        ibuf, 0,
        QRhiCommandBuffer::IndexUInt32
        );

    cb->drawIndexed(mesh.indexCount());
}
