#include "Engine/Math/Rotator.h"

using namespace DirectX;

namespace Engine {
    XMVECTOR NormalizeEulerAngles(XMVECTOR xmEuler) {
        return xmEuler - 360.0f * XMVectorTruncate(xmEuler / 360.0f);
    }

    Rotator::Rotator() : XMFLOAT3(0.0f, 0.0f, 0.0f) {

    }

    Rotator::Rotator(float roll, float pitch, float yaw) : XMFLOAT3(roll, pitch, yaw) {

    }

    Quaternion Rotator::ToQuatertion() const {
        XMVECTOR xmVector = XMLoadFloat3(this) * RADIAN_COEFFICIENT;
        XMVECTOR xmQuaternion = XMQuaternionRotationRollPitchYawFromVector(xmVector);

        Quaternion result;
        XMStoreFloat4(&result, xmQuaternion);

        return result;
    }

    Vector3 Rotator::RotateVector(const Vector3 vector) {
        return ToQuatertion() * vector;
    }

    Vector3 Rotator::UnRotateVector(const Vector3 vector) {
        return ToQuatertion().Inverse() * vector;
    }

    Rotator Rotator::operator+(const Rotator& rotator) const {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&rotator);
        XMVECTOR xmVector = xmVector1 + xmVector2;

        Rotator result;
        XMStoreFloat3(&result, NormalizeEulerAngles(xmVector));

        return result;
    }

    Rotator Rotator::operator-(const Rotator& rotator) const {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&rotator);
        XMVECTOR xmVector = xmVector1 - xmVector2;

        Rotator result;
        XMStoreFloat3(&result, NormalizeEulerAngles(xmVector));

        return result;
    }

    Rotator Rotator::operator*(Float value) const {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector = xmVector1 * value;

        Rotator result;
        XMStoreFloat3(&result, NormalizeEulerAngles(xmVector));

        return result;
    }

    Rotator Rotator::operator/(Float value) const {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector = xmVector1 / value;

        Rotator result;
        XMStoreFloat3(&result, NormalizeEulerAngles(xmVector));

        return result;
    }
}
