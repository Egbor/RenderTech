#ifndef MESHATTRIBUTES_H
#define MESHATTRIBUTES_H

#include "Engine/Rendering/MeshDescription.h"

namespace Engine {
    template<class ArrayType, class AttributeType, class ElementIDType>
    class MeshAttributeRef {
    private:
        ArrayType* m_ptrArray;

    public:
        MeshAttributeRef(ArrayType* ptrArray) : m_ptrArray(ptrArray) {}
        MeshAttributeRef(const MeshAttributeRef&) = default;
        virtual ~MeshAttributeRef() = default;

        AttributeType& operator[](ElementIDType index) {
            return *((AttributeType*)((UInt8*)&m_ptrArray[index]));
        }
    };

    template<class AttributeType>
    using VertexAttributeRef = MeshAttributeRef<MeshVertex, AttributeType, VertexID>;
    template<class AttributeType>
    using FaceAttributeRef = MeshAttributeRef<MeshFace, AttributeType, FaceID>;

    class MeshAttributes {
    private:
        MeshDescription& m_desc;

    public:
        MeshAttributes(MeshDescription& desc);
        MeshAttributes(const MeshAttributes&) = default;
        virtual ~MeshAttributes() = default;

        VertexAttributeRef<Vector3> GetVertexPositions();
        VertexAttributeRef<Vector3> GetVertexTangents();
        VertexAttributeRef<Vector3> GetVertexBitangents();
        VertexAttributeRef<Vector3> GetVertexNormals();
        VertexAttributeRef<Vector2> GetVertexUVs();
    };
}

#endif // MESHATTRIBUTES_H
