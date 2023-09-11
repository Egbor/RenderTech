#ifndef ENTITYCOMPONENTBEHAVIOR_H
#define ENTITYCOMPONENTBEHAVIOR_H

#include "Engine/Object/Global/ECBStorage.h"
#include "Engine/Object/Component/EntityComponent.h"

namespace Engine {
    template<class EntityComponentType>
    CLASSTYPE(EntityComponentBehavior, GENERIC)

    template<class EntityComponentType>
    class EntityComponentBehavior : public Object, public IEntityComponentBehavior {
        GENERATE_BODY(EntityComponentBehavior<EntityComponentType>, Object)

    private:
        List<EntityComponentType*> m_components;

    public:
        EntityComponentBehavior(const ObjectArgument& argument)
            : Super(argument), IEntityComponentBehavior() {
            ECBStorage::GetInstance().AddNewComponentBehavior(this);
        }

        virtual ~EntityComponentBehavior() {
            while(m_components.begin() != m_components.end()) {
                UnRegisterComponent((*m_components.begin()));
            }
            ECBStorage::GetInstance().DeleteComponentBehavior(Id());
        }

        EntityComponentBehavior(const EntityComponentBehavior<EntityComponentType>&) = default;

        HandlerPtr RegisterComponent(EntityComponent* component) override {
            m_components.push_back(static_cast<EntityComponentType*>(component));
            return new ListIterator<EntityComponentType*>(--m_components.end());
        }

        void UnRegisterComponent(EntityComponent* component) override {
            if (component->GetHandler() != nullptr) {
                ListIterator<EntityComponentType*>* it = static_cast<ListIterator<EntityComponentType*>*>(component->GetHandler());
                m_components.erase(*it);
                DELETE_OBJECT(it);
            }
        }

        Int64 Id() const override {
            return EntityComponentType::TypeIdClass();
        }

    protected:
        const List<EntityComponentType*>& GetComponents() const {
            return m_components;
        }
    };

    GENERATE_RTTI_DEFINITIONS_TEMPLATED(EntityComponentBehavior)
}

#endif // ENTITYCOMPONENTBEHAVIOR_H
