#include "Scripts/VisibleObject.h"

#include "Engine/Core/Core.h"
#include "Engine/Object/Class/Material.h"

#include <cmath>

namespace Engine {
    GENERATE_INSTANTIATION(VisibleObject)

    VisibleObject::VisibleObject(const ObjectArgument& argument)
        : Super(argument) {
        Mesh* mesh = Core::Load<Mesh>("models/Cyberpunk_Robot.obj"); // Resource::Load<Mesh*>("assets/models/Cyberpunk_Robot.obj");
        Material* material = Core::Load<Material>("materials/Cyberpunk_Robot.xml"); // Resource::Load<Material*>("assets/materials/Cyberpunk_Robot.xml");
        mesh->SetMaterial(0, material);

        m_meshComponent = ClassType<MeshComponent>::CreateObject(ObjectArgument::Dummy());
        m_meshComponent->AttachToComponent(GetRootComponent());
        m_meshComponent->SetMesh(mesh);

        SetEntityScale(Vector3(0.005f, 0.005f, 0.005f));
    }

    VisibleObject::~VisibleObject() {
        DELETE_OBJECT(m_meshComponent);
    }

    void VisibleObject::OnUpdate(Float deltaTime) {
        // AddEntityRotation(Rotator(0.0f, -60.0f * deltaTime, 0.0f));
    }
}
