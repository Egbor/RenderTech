#include "Engine/Math/Matrix.h"

using namespace DirectX;

namespace Engine {
    Matrix4x4 Matrix4x4::CreateMatrixPerspective(Float fovAngle, Float aspectRatio, Float nearZ, Float farZ) {
        XMMATRIX xmMatrix = XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearZ, farZ);

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmMatrix);

        return result;
    }

    Matrix4x4 Matrix4x4::CreateMatrixOrthographic(Float viewLeft, Float viewRight, Float viewBottom, Float viewTop, Float nearZ, Float farZ) {
        XMMATRIX xmMatrix = XMMatrixOrthographicOffCenterLH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmMatrix);

        return result;
    }

    Matrix4x4 Matrix4x4::CreateMatrixLookAt(Vector3 eyePosition, Vector3 focusPosition, Vector3 upDirection) {
        XMVECTOR xmEyePosition = XMLoadFloat3(&eyePosition);
        XMVECTOR xmFocusPosition = XMLoadFloat3(&focusPosition);
        XMVECTOR xmUpDirection = XMLoadFloat3(&upDirection);

        XMMATRIX xmMatrix = XMMatrixLookAtLH(xmEyePosition, xmFocusPosition, xmUpDirection);

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmMatrix);

        return result;
    }

    Matrix4x4 Matrix4x4::CreateMatrixWorld(Vector3 position, Rotator rotation, Vector3 scale) {
        XMVECTOR xmPosition = XMLoadFloat3(&position);
        XMVECTOR xmRotation = XMLoadFloat3(&rotation) * RADIAN_COEFFICIENT;
        XMVECTOR xmScale = XMLoadFloat3(&scale);

        XMMATRIX xmMatrixTranslation = XMMatrixTranslationFromVector(xmPosition);
        XMMATRIX xmMatrixRotation = XMMatrixRotationRollPitchYawFromVector(xmRotation);
        XMMATRIX xmMatrixScaling = XMMatrixScalingFromVector(xmScale);

        XMMATRIX xmMatrix = xmMatrixScaling * xmMatrixRotation * xmMatrixTranslation;

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmMatrix);

        return result;
    }

    Matrix4x4 Matrix4x4::Identity() {
        XMMATRIX xmIdentity = XMMatrixIdentity();

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmIdentity);

        return result;
    }

    Matrix4x4 Matrix4x4::Transpose() {
        XMMATRIX xmTransposedMatrix = XMMatrixTranspose(XMLoadFloat4x4(this));

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmTransposedMatrix);

        return result;
    }

    Matrix4x4 Matrix4x4::Inverse() {
        XMMATRIX xmInverseMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(this));

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmInverseMatrix);

        return result;
    }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& matrix) {
        XMMATRIX xmMatrix1 = XMLoadFloat4x4(this);
        XMMATRIX xmMatrix2 = XMLoadFloat4x4(&matrix);
        XMMATRIX xmMatrix = XMMatrixMultiply(xmMatrix1, xmMatrix2);

        Matrix4x4 result;
        XMStoreFloat4x4(&result, xmMatrix);

        return result;
    }

    void Matrix4x4::operator*=(const Matrix4x4& matrix) {
        Matrix4x4 result = *this * matrix;
        memcpy_s(this, sizeof(Matrix4x4), &result, sizeof(Matrix4x4));
    }
}
