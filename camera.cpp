#include "camera.h"
#include "math3d.h"

Mat4 Camera::getViewMatrix() const {
    Mat4 R = quatToMat4(orientation);

    Mat4 view = R;

    std::swap(view.m[1], view.m[4]);
    std::swap(view.m[2], view.m[8]);
    std::swap(view.m[6], view.m[9]);

    Vec3 r = getRight(), u = getUp(), f = getForward();
    view.m[12] = -dot(r, position);
    view.m[13] = -dot(u, position);
    view.m[14] = -dot(f, position);
    view.m[15] = 1.0f;

    return view;
}

Mat4 Camera::getViewProjectionMatrix(int width, int height) const {
    Mat4 proj = perspective(fov, float(width) / height, 0.1f, 160.0f);
    return proj * getViewMatrix();
}

void Camera::rotate(const Vec3& worldAxis, float angleDeg) {
    Quat invOrientation = {orientation.w, -orientation.x, -orientation.y, -orientation.z};
    Vec3 localAxis = rotateVec(invOrientation, worldAxis);

    float rad = angleDeg * M_PI / 180.0f;
    Quat delta = Quat::fromAxisAngle(localAxis, rad);

    orientation = normalizeQ(orientation * delta);
}

void Camera::move(const Vec3& localDelta) {
    position = position + getRight() * localDelta.x
               + getUp() * localDelta.y
               + getForward() * localDelta.z;
}

void Camera::snapPosition() {
    const float grid = 0.5f;
    position.x = round(position.x / grid) * grid;
    position.y = round(position.y / grid) * grid;
    position.z = round(position.z / grid) * grid;
}

void Camera::snapRotation() {
    Vec3 forward = getForward();

    Vec3 snappedForward;
    float absX = fabs(forward.x), absY = fabs(forward.y), absZ = fabs(forward.z);

    if (absX > absY && absX > absZ)
        snappedForward = {forward.x > 0 ? 1.0f : -1.0f, 0, 0};
    else if (absY > absX && absY > absZ)
        snappedForward = {0, forward.y > 0 ? 1.0f : -1.0f, 0};
    else
        snappedForward = {0, 0, forward.z > 0 ? 1.0f : -1.0f};

    Vec3 worldUp = {0, 1, 0};
    if (fabs(dot(snappedForward, worldUp)) > 0.99f)
        worldUp = {0, 0, 1};

    Vec3 snappedRight = normalize(cross(worldUp, snappedForward));
    Vec3 snappedUp = cross(snappedForward, snappedRight);

    Vec3 currentUp = getUp();
    float sinRoll = dot(cross(snappedUp, currentUp), snappedForward);
    float cosRoll = dot(snappedUp, currentUp);
    float roll = round(atan2(sinRoll, cosRoll) / (M_PI * 0.5f)) * (M_PI * 0.5f);

    float c = cos(roll), s = sin(roll);
    Vec3 finalUp = snappedUp * c - snappedRight * s;
    Vec3 finalRight = cross(finalUp, snappedForward);

    Mat4 m;
    m.m[0] = finalRight.x; m.m[4] = finalUp.x; m.m[8] = snappedForward.x;
    m.m[1] = finalRight.y; m.m[5] = finalUp.y; m.m[9] = snappedForward.y;
    m.m[2] = finalRight.z; m.m[6] = finalUp.z; m.m[10] = snappedForward.z;
    m.m[15] = 1.0f;

    orientation = mat4ToQuat(m);
}
