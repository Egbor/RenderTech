#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "Engine/Tools/Event.h"

#include "Engine/Object/Component/EntityComponent.h"

namespace Engine {
    class InputComponent : public EntityComponent {
        GENERATE_BODY(InputComponent, EntityComponent)

    private:
        Map<String, std::unique_ptr<Event<>>> m_actions;
        Map<String, std::unique_ptr<Event<Float>>> m_axis;

    public:
        InputComponent(const ObjectArgument& arguments);
        virtual ~InputComponent() = default;

        template<class TOwner>
        bool BindAction(const String& tag, TOwner* owner, typename Method<TOwner>::TMethodPtr callback) {
            MapIterator<String, std::unique_ptr<Event<>>> it = m_actions.find(tag);
            if (it == m_actions.end()) {
                m_actions.insert({ tag, std::make_unique<Event<>>() });
            }
            (*m_actions[tag]) += Delegate<TOwner>::Allocate(owner, callback);
        }

        template<class TOwner>
        void BindAxis(const String& tag, TOwner* owner, typename Method<TOwner, Float>::TMethodPtr callback) {
            MapIterator<String, std::unique_ptr<Event<Float>>> it = m_axis.find(tag);
            if (it == m_axis.end()) {
                m_axis.insert({ tag, std::make_unique<Event<Float>>() });
            }
            (*m_axis[tag]) += Delegate<TOwner, Float>::Allocate(owner, callback);
        }

        template<class Predicate>
        void ForEachAction(Predicate lambda) {
            for (auto it = m_actions.begin(); it != m_actions.end(); it++) {
                lambda(it->first, *it->second.get());
            }
        }

        template<class Predicate>
        void ForEachAxis(Predicate lambda) {
            for (auto it = m_axis.begin(); it != m_axis.end(); it++) {
                lambda(it->first, *it->second.get());
            }
        }

        void ResetBinds();
    };
}

#endif // INPUTCOMPONENT_H
