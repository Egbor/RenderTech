#ifndef MATRIX_H
#define MATRIX_H

#include "Engine/Math/Vector.h"
#include "Engine/Math/Rotator.h"

namespace Engine {
    struct Matrix4x4 : public DirectX::XMFLOAT4X4 {
        Matrix4x4() = default;
        Matrix4x4(const Matrix4x4&) = default;

        static Matrix4x4 CreateMatrixOrthographic(Float viewLeft, Float viewRight, Float viewBottom, Float viewTop, Float nearZ, Float farZ);
        static Matrix4x4 CreateMatrixPerspective(Float fovAngle, Float aspectRatio, Float nearZ, Float farZ);
        static Matrix4x4 CreateMatrixLookAt(Vector3 eyePosition, Vector3 focusPosition, Vector3 upDirection);
        static Matrix4x4 CreateMatrixWorld(Vector3 position, Rotator rotation, Vector3 scale);

        Matrix4x4 Transpose();
        Matrix4x4 Inverse();

        Matrix4x4 operator+(const Matrix4x4& matrix);
        Matrix4x4 operator-(const Matrix4x4& matrix);
        Matrix4x4 operator*(const Matrix4x4& matrix);

        void operator+=(const Matrix4x4& matrix);
        void operator-=(const Matrix4x4& matrix);
        void operator*=(const Matrix4x4& matrix);

        static Matrix4x4 Identity();
    };
}

#endif // MATRIX_H
