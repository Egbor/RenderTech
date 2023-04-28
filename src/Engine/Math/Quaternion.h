#ifndef QUATERNION_H
#define QUATERNION_H

#include "Engine/Math/Vector.h"

namespace Engine {
    struct Quaternion : public DirectX::XMFLOAT4 {
        Quaternion();
        Quaternion(float w, float x, float y, float z);
        Quaternion(const Quaternion&) = default;

        Quaternion Inverse();

        Vector3 operator*(const Vector3& vector) const;
        Quaternion operator*(const Quaternion& quaternion) const;
        bool operator==(const Quaternion& quaternion);
    };
}

#endif // QUATERNION_H
