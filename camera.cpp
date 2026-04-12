#include "camera.h"
#include "math3d.h"

void Camera::rotate(const Vec3& axis, float angleDeg)
{
    Mat4 R = rotationMatrix(axis, angleDeg);

    transform = transform * R;  // local-space rotation
}

Mat4 Camera::getViewMatrix() const
{
    return inverseRigid(transform);
}

void Camera::move(const Vec3& d)
{
    Vec4 local = {d.x, d.y, d.z, 1.0f};
    Vec4 world = transform * local;

    transform.m[12] = world.x;
    transform.m[13] = world.y;
    transform.m[14] = world.z;
}

void Camera::snapPosition()
{
    transform.m[12] = round(transform.m[12]*2)/2;
    transform.m[13] = round(transform.m[13]*2)/2;
    transform.m[14] = round(transform.m[14]*2)/2;
}

void Camera::snapRotation()
{
    Mat4 R = transform;

    // extract basis vectors
    Vec3 forward = normalize(Vec3{R.m[8], R.m[9], R.m[10]});

    // snap forward direction (optional, coarse)
    forward.x = round(forward.x);
    forward.y = round(forward.y);
    forward.z = round(forward.z);

    forward = normalize(forward);

    Vec3 up = {0,1,0};
    Vec3 right = normalize(cross(forward, up));
    up = cross(right, forward);

    // rebuild rotation in matrix
    R.m[0] = right.x;   R.m[4] = up.x;   R.m[8]  = forward.x;
    R.m[1] = right.y;   R.m[5] = up.y;   R.m[9]  = forward.y;
    R.m[2] = right.z;   R.m[6] = up.z;   R.m[10] = forward.z;

    transform = R;
}

Vec3 Camera::getRight(const Mat4& m)
{
    return {m.m[0], m.m[1], m.m[2]};
}

Vec3 Camera::getUp(const Mat4& m)
{
    return {m.m[4], m.m[5], m.m[6]};
}

Vec3 Camera::getForward(const Mat4& m)
{
    return {m.m[8], m.m[9], m.m[10]};
}
