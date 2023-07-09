#include "Engine/Object/Component/MeshComponent.h"

#include "Engine/Core/System/Import/MeshImport.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(MeshComponent)
    GENERATE_INSTANTIATION(MeshComponent)

    MeshComponent::MeshComponent(const ObjectArgument& argument)
        : Super(argument), m_mesh(nullptr) {
    }

    void MeshComponent::SetMesh(Mesh* mesh) {
        m_mesh = mesh;
    }

    Mesh* MeshComponent::GetMesh() const {
        return m_mesh;
    }

    UInt64 MeshComponent::GetBehaviorID() const {
        return MeshComponent::TypeIdClass();
    }

    void MeshComponent::Serialize(ISerializer* serializer) {

    }

    void MeshComponent::Deserialize(ISerializer* serializer) {
        Super::Deserialize(serializer);

        String path = "";
        serializer->GetString(argTagPath, path);

        MeshImport importer(path);
        m_mesh = importer.LoadResource();


        ISerializer* subobjectSerializer = nullptr;
        while (serializer->CreateSubobjectSerializer(&subobjectSerializer)) {
            Object* object = ObjectType::CreateDefaultObjectByName<Object>(subobjectSerializer->GetObjectName());

            if (object->Is(Material::TypeIdClass())) {
                Material* material = object->As<Material>();

                Int32 id = -1;
                String path = "";

                ISerializer* materialSerializer = nullptr;
                ISerializer* materialRoot = nullptr;
                subobjectSerializer->GetInt32(argTagId, id);
                subobjectSerializer->GetString(argTagPath, path);
                subobjectSerializer->CreateSubobjectSerializer(path, &materialSerializer);  
                materialSerializer->CreateSubobjectSerializer(&materialRoot);
                material->Deserialize(materialRoot);
                materialSerializer->DispatchSubobjectSerializer(&materialRoot);
                subobjectSerializer->DispatchSubobjectSerializer(&materialSerializer);

                m_mesh->SetMaterial(id, material);
            } else {
                DELETE_OBJECT(object);
            }
        }
        serializer->DispatchSubobjectSerializer(&subobjectSerializer);
    }
}
