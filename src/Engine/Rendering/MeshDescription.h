#ifndef MESHDESCRIPTION_H
#define MESHDESCRIPTION_H

#include "Engine/Rendering/Types.h"
#include "Engine/Math/Vector.h"

namespace Engine {
    struct MeshVertex {
        Vector3 position;
        Vector3 tangent;
        Vector3 bitangent;
        Vector3 normal;
        Vector2 uv;

        MeshVertex();
        MeshVertex(const MeshVertex&) = default;
        ~MeshVertex() = default;
    };

    struct MeshFace {
        VertexID vertices[3];

        MeshFace(const Array<VertexID>& vertexIDs);
        MeshFace(const MeshFace&) = default;
        ~MeshFace() = default;

        static constexpr UInt32 countOfVertices = sizeof(vertices) / sizeof(vertices[0]);
    };

    class MeshDescription {
    private:
        Array<MeshVertex> m_vertices;
        Array<MeshFace> m_faces;

    public:
        MeshDescription();
        MeshDescription(const MeshDescription&) = default;
        virtual ~MeshDescription() = default;

        VertexID CreateVertex();
        FaceID CreateFace(Array<VertexID> vertexIDs);

        Array<MeshVertex>& Vertices();
        Array<MeshFace>& Faces();

        BufferInfo GetMeshInfo();
        BufferInfo GetFaceInfo();
    };
}

#endif // MESHDESCRIPTION_H
