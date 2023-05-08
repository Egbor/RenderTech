#include "Scripts/LightObject.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(LightObject)

    LightObject::LightObject(const ObjectArgument& argument)
        : Super(argument) {
        m_lightComponent1 = CreateDefaultSubobject<PointLightComponent>("PointLightComponent");
        m_lightComponent2 = CreateDefaultSubobject<PointLightComponent>("PointLightComponent");
        m_lightComponent3 = CreateDefaultSubobject<PointLightComponent>("PointLightComponent");
        m_lightComponent4 = CreateDefaultSubobject<PointLightComponent>("PointLightComponent");

        m_lightComponent1->AttachToComponent(GetRootComponent());
        m_lightComponent2->AttachToComponent(GetRootComponent());
        m_lightComponent3->AttachToComponent(GetRootComponent());
        m_lightComponent4->AttachToComponent(GetRootComponent());

        m_lightComponent1->SetColor(Vector4(1.0f, 1.0f/*0.443f*/, 1.0f/*0.808f*/, 1.0f));
        m_lightComponent2->SetColor(Vector4(1.0f/*0.004f*/, 1.0f/*0.804f*/, 1.0f/*0.996f*/, 1.0f));
        m_lightComponent3->SetColor(Vector4(1.0f/*0.02f*/, 1.0f, 1.0f/*0.631f*/, 1.0f));
        m_lightComponent4->SetColor(Vector4(1.0f/*0.725f*/, 1.0f/*0.404f*/, 1.0f, 1.0f));

        m_lightComponent1->SetLocalPosition(Vector3(0.354f, 0.0f, -0.354f));
        m_lightComponent2->SetLocalPosition(Vector3(-0.354f, 0.0f, -0.354f));
        m_lightComponent3->SetLocalPosition(Vector3(0.354f, 0.0f, 0.354f));
        m_lightComponent4->SetLocalPosition(Vector3(-0.354f, 0.0f, 0.354f));

        //SetEntityScale(Vector3(0.5f, 0.5f, 0.5f));
        //SetEntityLocation(Vector3(0.0f, 0.0f, -0.1f));
    }

    LightObject::~LightObject() {
        DELETE_OBJECT(m_lightComponent4);
        DELETE_OBJECT(m_lightComponent3);
        DELETE_OBJECT(m_lightComponent2);
        DELETE_OBJECT(m_lightComponent1);
    }

    void LightObject::OnUpdate(Float deltaTime) {
        //AddEntityRotation(Rotator(0.0f, 30.0f * deltaTime, 0.0f));
    }
}
