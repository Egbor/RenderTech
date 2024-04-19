#include "Scripts/VisibleObject.h"

#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Object/Class/Material.h"

#include <cmath>

namespace Engine {
    GENERATE_INSTANTIATION(VisibleObject)

    VisibleObject::VisibleObject(const ObjectArgument& argument)
        : Super(argument) {
        m_meshComponent = ClassType<MeshComponent>::CreateObject(ObjectArgument::Dummy());
        m_meshComponent->AttachToComponent(GetRootComponent());

        Mesh* mesh = Resource::Load<Mesh*>("assets/models/Cyberpunk_Robot.obj");
        Material* material = Resource::Load<Material*>("assets/materials/Cyberpunk_Robot.xml");
        mesh->SetMaterial(0, material);

        m_meshComponent->SetMesh(mesh);

        SetEntityScale(Vector3(0.005f, 0.005f, 0.005f));
    }

    VisibleObject::~VisibleObject() {

    }

    void VisibleObject::OnUpdate(Float deltaTime) {
        // AddEntityRotation(Rotator(0.0f, -60.0f * deltaTime, 0.0f));
    }
}
