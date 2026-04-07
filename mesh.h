class Mesh {
public:
    std::vector<Vec3> vertices;
    std::vector<uint32_t> indices;

    Mat4 getModelMatrix() const;
    int indexCount() const;
};
