#include "Engine/Math/Quaternion.h"

using namespace DirectX;

namespace Engine {
    Quaternion::Quaternion() : XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {

    }

    Quaternion::Quaternion(float w, float x, float y, float z) : XMFLOAT4(x, y, z, w) {

    }

    Quaternion Quaternion::Inverse() {
       XMVECTOR xmQuaternion = XMQuaternionInverse(XMLoadFloat4(this));

       Quaternion result;
       XMStoreFloat4(&result, xmQuaternion);

       return result;
    }

    Vector3 Quaternion::operator*(const Vector3& vector) const {
        XMVECTOR xmQuaternion1 = XMLoadFloat4(this);
//        XMVECTOR xmQuaternion2 = XMQuaternionConjugate(xmQuaternion1);
        XMVECTOR xmVector1 = XMLoadFloat3(&vector);
        XMVECTOR xmVector = XMVector3Rotate(xmVector1, xmQuaternion1);//XMQuaternionMultiply(xmQuaternion2, XMQuaternionMultiply(xmVector1, xmQuaternion1));

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    Quaternion Quaternion::operator*(const Quaternion& quaternion) const {
        XMVECTOR xmQuaternion1 = XMLoadFloat4(this);
        XMVECTOR xmQuaternion2 = XMLoadFloat4(&quaternion);
        XMVECTOR xmQuaternion = XMQuaternionMultiply(xmQuaternion2, xmQuaternion1);

        Quaternion result;
        XMStoreFloat4(&result, xmQuaternion);

        return result;
    }

    bool Quaternion::operator==(const Quaternion& quaternion) {
        return (this->w == quaternion.w) && (this->x == quaternion.x) && (this->y == quaternion.y) && (this->z == quaternion.z);
    }
}
