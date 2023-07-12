#ifndef ROTATOR_H
#define ROTATOR_H

#include "Engine/Math/Quaternion.h"

namespace Engine {
    STRUCTTYPE(Rotator)
    struct Rotator : DirectX::XMFLOAT3 {
        Rotator();
        Rotator(float roll, float pitch, float yaw);
        Rotator(const Rotator&) = default;

        Quaternion ToQuatertion() const;

        Vector3 RotateVector(const Vector3 vector);
        Vector3 UnRotateVector(const Vector3 vector);

        Rotator operator+(const Rotator& rotator) const;
        Rotator operator-(const Rotator& rotator) const;

        Rotator operator*(Float value) const;
        Rotator operator/(Float value) const;
    };
}

#endif // ROTATOR_H
