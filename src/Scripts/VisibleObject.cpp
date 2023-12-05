#include "Scripts/VisibleObject.h"

#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Object/Class/Material.h"

#include <cmath>

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(VisibleObject)
    GENERATE_INSTANTIATION(VisibleObject)

    VisibleObject::VisibleObject(const ObjectArgument& argument)
        : Super(argument) {
        m_meshComponent = ClassType<MeshComponent>::CreateObject(ObjectArgument::Dummy());
        m_meshComponent->AttachToComponent(GetRootComponent());

        Mesh* mesh = Resource::Load<Mesh*>("assets/models/SM_Gameboy.fbx");
        Material* material = Resource::Load<Material*>("assets/materials/SM_Gameboy.xml");
        mesh->SetMaterial(0, material);

        m_meshComponent->SetMesh(mesh);

        //SetEntityLocation(Vector3(0.0f, -0.1f, -0.0f));
        SetEntityScale(Vector3(0.0009f, 0.0009f, 0.0009f));
    }

    VisibleObject::~VisibleObject() {

    }

    void VisibleObject::OnUpdate(Float deltaTime) {
        AddEntityRotation(Rotator(0.0f, -60.0f * deltaTime, 0.0f));
    }
}
