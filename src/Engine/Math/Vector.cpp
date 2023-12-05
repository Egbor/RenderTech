#include "Engine/Math/Vector.h"

using namespace DirectX;

namespace Engine {
    Vector2::Vector2() : XMFLOAT2(0.0f, 0.0f) {

    }

    Vector2::Vector2(float x, float y) : XMFLOAT2(x, y) {

    }

    Vector3 Vector3::forword = Vector3(0.0f, 0.0f, 1.0f);
    Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
    Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);

    Vector3::Vector3() : XMFLOAT3(0.0f, 0.0f, 0.0f) {

    }

    Vector3::Vector3(float x, float y, float z) : XMFLOAT3(x, y, z) {

    }

    Vector3 Vector3::Normalize() {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmNormalizedVector = XMVector3Normalize(xmVector1);

        Vector3 result;
        XMStoreFloat3(&result, xmNormalizedVector);

        return result;
    }

    Vector3 Vector3::Cross(const Vector3& vector) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&vector);
        XMVECTOR xmCrossVector = XMVector3Cross(xmVector1, xmVector2);

        Vector3 result;
        XMStoreFloat3(&result, xmCrossVector);

        return result;
    }

    float Vector3::Dot(const Vector3& vector) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&vector);
        XMVECTOR xmDotVector = XMVector3Dot(xmVector1, xmVector2);

        Vector3 result;
        XMStoreFloat3(&result, xmDotVector);

        return result.x;
    }

    float Vector3::Abs() {
        XMVECTOR xmVector = XMLoadFloat3(this);
        XMVECTOR xmAbsVector = XMVectorAbs(xmVector);

        Vector3 result;
        XMStoreFloat3(&result, xmAbsVector);

        return result.x;
    }

    Vector3 Vector3::operator-() {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector = -xmVector1;

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    Vector3 Vector3::operator+(const Vector3& vector) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&vector);
        XMVECTOR xmVector = xmVector1 + xmVector2;

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    Vector3 Vector3::operator-(const Vector3& vector) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&vector);
        XMVECTOR xmVector = xmVector1 - xmVector2;

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    Vector3 Vector3::operator*(const Vector3& vector) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector2 = XMLoadFloat3(&vector);
        XMVECTOR xmVector = xmVector1 * xmVector2;

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    Vector3 Vector3::operator+(const float arg) {
        return *this + Vector3(arg, arg, arg);
    }

    Vector3 Vector3::operator-(const float arg) {
        return *this - Vector3(arg, arg, arg);
    }

    Vector3 Vector3::operator*(const float arg) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector = xmVector1 * arg;

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    Vector3 Vector3::operator/(const float arg) {
        XMVECTOR xmVector1 = XMLoadFloat3(this);
        XMVECTOR xmVector = xmVector1 * arg;

        Vector3 result;
        XMStoreFloat3(&result, xmVector);

        return result;
    }

    void Vector3::operator+=(const Vector3& vector) {
        *this = *this + vector;
    }

    void Vector3::operator-=(const Vector3& vector) {
        *this = *this - vector;
    }

    void Vector3::operator+=(const float arg) {
        *this = *this + arg;
    }

    void Vector3::operator-=(const float arg) {
        *this = *this + arg;
    }

    void Vector3::operator*=(const float arg) {
        *this = *this * arg;
    }

    void Vector3::operator/=(const float arg) {
        *this = *this + arg;
    }

    bool Vector3::operator==(const Vector3& vector) {
        return (this->x == vector.x) && (this->y == vector.y) && (this->z == vector.z);
    }

    Vector4::Vector4() : XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {

    }

    Vector4::Vector4(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {

    }

    Vector4 Vector4::operator+(const Vector4& vector) {
        XMVECTOR xmVector1 = XMLoadFloat4(this);
        XMVECTOR xmVector2 = XMLoadFloat4(&vector);
        XMVECTOR xmVector = xmVector1 + xmVector2;

        Vector4 result;
        XMStoreFloat4(&result, xmVector);

        return result;
    }

    Vector4 Vector4::operator-(const Vector4& vector) {
        XMVECTOR xmVector1 = XMLoadFloat4(this);
        XMVECTOR xmVector2 = XMLoadFloat4(&vector);
        XMVECTOR xmVector = xmVector1 - xmVector2;

        Vector4 result;
        XMStoreFloat4(&result, xmVector);

        return result;
    }

    Vector4 Vector4::operator+(const float arg) {
        return *this + Vector4(arg, arg, arg, arg);
    }

    Vector4 Vector4::operator-(const float arg) {
        return *this - Vector4(arg, arg, arg, arg);
    }

    Vector4 Vector4::operator*(const float arg) {
        XMVECTOR xmVector1 = XMLoadFloat4(this);
        XMVECTOR xmVector = xmVector1 * arg;

        Vector4 result;
        XMStoreFloat4(&result, xmVector);

        return result;
    }

    Vector4 Vector4::operator/(const float arg) {
        XMVECTOR xmVector1 = XMLoadFloat4(this);
        XMVECTOR xmVector = xmVector1 * arg;

        Vector4 result;
        XMStoreFloat4(&result, xmVector);

        return result;
    }

    Vector4 Vector4::operator+=(const Vector4& vector) {
        return *this + vector;
    }

    Vector4 Vector4::operator-=(const Vector4& vector) {
        return *this - vector;
    }

    Vector4 Vector4::operator+=(const float arg) {
        return *this + arg;
    }

    Vector4 Vector4::operator-=(const float arg) {
        return *this + arg;
    }

    Vector4 Vector4::operator*=(const float arg) {
        return *this * arg;
    }

    Vector4 Vector4::operator/=(const float arg) {
        return *this + arg;
    }

    bool Vector4::operator==(const Vector4& vector) {
        return (this->x == vector.x) && (this->y == vector.y) && (this->z == vector.z);
    }
}
