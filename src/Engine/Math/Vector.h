#ifndef VECTOR_H
#define VECTOR_H

#include "Engine/Math/MathDef.h"

namespace Engine {
    struct Vector2 : public DirectX::XMFLOAT2 {
        Vector2();
        Vector2(float x, float y);
        Vector2(const Vector2&) = default;

        Vector2 Normalize();
        float Dot(const Vector2& vector);

        Vector2 operator+(const Vector2& vector);
        Vector2 operator-(const Vector2& vector);

        Vector2 operator+(const float arg);
        Vector2 operator-(const float arg);
        Vector2 operator*(const float arg);
        Vector2 operator/(const float arg);

        void operator+=(const Vector2& vector);
        void operator-=(const Vector2& vector);

        bool operator==(const Vector2& vector);
    };

    STRUCTTYPE(Vector3)
    struct Vector3 : public DirectX::XMFLOAT3 {
        Vector3();
        Vector3(float x, float y, float z);
        Vector3(const Vector3&) = default;

        Vector3 Normalize();
        Vector3 Cross(const Vector3& vector);
        float Dot(const Vector3& vector);

        Vector3 operator-();

        Vector3 operator+(const Vector3& vector);
        Vector3 operator-(const Vector3& vector);
        Vector3 operator*(const Vector3& vector);

        Vector3 operator+(const float arg);
        Vector3 operator-(const float arg);
        Vector3 operator*(const float arg);
        Vector3 operator/(const float arg);

        void operator+=(const Vector3& vector);
        void operator-=(const Vector3& vector);

        void operator+=(const float arg);
        void operator-=(const float arg);
        void operator*=(const float arg);
        void operator/=(const float arg);

        bool operator==(const Vector3& vector);

        static Vector3 forword;
        static Vector3 right;
        static Vector3 up;
        static Vector3 zero;
    };

    STRUCTTYPE(Vector4)
    struct Vector4 : public DirectX::XMFLOAT4 {
        Vector4();
        Vector4(float x, float y, float z, float w);
        Vector4(const Vector4&) = default;

        Vector4 operator+(const Vector4& vector);
        Vector4 operator-(const Vector4& vector);

        Vector4 operator+(const float arg);
        Vector4 operator-(const float arg);
        Vector4 operator*(const float arg);
        Vector4 operator/(const float arg);

        Vector4 operator+=(const Vector4& vector);
        Vector4 operator-=(const Vector4& vector);

        Vector4 operator+=(const float arg);
        Vector4 operator-=(const float arg);
        Vector4 operator*=(const float arg);
        Vector4 operator/=(const float arg);

        bool operator==(const Vector4& vector);
    };
}

#endif // VECTOR_H
