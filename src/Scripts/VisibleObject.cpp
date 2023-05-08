#include "Scripts/VisibleObject.h"

#include "Engine/Core/System/Import/MeshImport.h"
#include "Engine/Core/System/Import/TextureImport.h"

#include "Engine/Object/Class/Material.h"

#include <cmath>

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(VisibleObject)

    Texture2D* albedo;
    Texture2D* normal;
    Texture2D* metallic;
    Texture2D* roughness;
    Texture2D* ao;

    VisibleObject::VisibleObject(const ObjectArgument& argument)
        : Super(argument) {
        m_meshComponent = CreateDefaultSubobject<MeshComponent>("MeshComponent");
        m_meshComponent->AttachToComponent(GetRootComponent());

        MeshImport meshImport("./bin/models/SM_Gameboy.fbx");
        TextureImport albedoImport("./bin/models/Gameboy_low_Gameboy_BaseColor.png");
        TextureImport normalImport("./bin/models/Gameboy_low_Gameboy_Normal.png");
        TextureImport metallicImport("./bin/models/Gameboy_low_Gameboy_Metallic.png");
        TextureImport roughnessImport("./bin/models/Gameboy_low_Gameboy_Roughness.png");
        TextureImport occlusionImport("./bin/models/Gameboy_low_Gameboy_AO.png");

        Mesh* mesh = meshImport.LoadResource();
        albedo = albedoImport.LoadResource();
        normal = normalImport.LoadResource();
        metallic = metallicImport.LoadResource();
        roughness = roughnessImport.LoadResource();
        ao = occlusionImport.LoadResource();

        Material* material = CreateDefaultSubobject<Material>();
        material->SetTexture(TextureSlot::TS_ALBEDO, albedo);
        material->SetTexture(TextureSlot::TS_NORMAL, normal);
        material->SetTexture(TextureSlot::TS_METALLIC, metallic);
        material->SetTexture(TextureSlot::TS_ROUGHNESS, roughness);
        material->SetTexture(TextureSlot::TS_AO, ao);

        m_meshComponent->SetMesh(mesh);
        m_meshComponent->GetMesh()->SetMaterial(0, material);

        SetEntityLocation(Vector3(0.0f, -0.1f, -0.0f));
        SetEntityScale(Vector3(0.0009f, 0.0009f, 0.0009f));
    }

    VisibleObject::~VisibleObject() {
        DELETE_OBJECT(ao);
        DELETE_OBJECT(albedo);
        DELETE_OBJECT(normal);
        DELETE_OBJECT(metallic);
        DELETE_OBJECT(roughness);
        DELETE_OBJECT(m_meshComponent);
    }

    void VisibleObject::OnUpdate(Float deltaTime) {
        AddEntityRotation(Rotator(0.0f, -60.0f * deltaTime, 0.0f));

//        Rotator curRotator = GetRootComponent()->GetLocalRotation();
//        SetEntityLocation(Vector3(0.0f, std::cosf(curRotator.y * 3.14f / 180.0f) * 0.01f, 0.0f));
    }


}
