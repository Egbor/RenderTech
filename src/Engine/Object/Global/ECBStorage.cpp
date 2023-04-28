#include "Engine/Object/Global/ECBStorage.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    void ECBStorage::AddNewComponentBehavior(IEntityComponentBehavior* behavior) {
        if (FindComponentBehaviorIterator(behavior->Id()) != m_behaviors.end()) {
            throw new EngineException("[ECBStorage] Duplicate component behavior creation");
        }
        m_behaviors.push_back(behavior);
    }

    void ECBStorage::DeleteComponentBehavior(UInt64 behaviorID) {
        auto it = FindComponentBehaviorIterator(behaviorID);
        if (it == m_behaviors.end()) {
            throw new EngineException("[ECBStorage] Try to delete nonexisted behavior");
        }
        m_behaviors.erase(it);
    }

    IEntityComponentBehavior* ECBStorage::FindComponentBehavior(UInt64 behaviorID) {
        auto it = FindComponentBehaviorIterator(behaviorID);
        if (it == m_behaviors.end()) {
            return nullptr;
        }
        return *it;
    }

    ArrayIterator<IEntityComponentBehavior*> ECBStorage::FindComponentBehaviorIterator(UInt64 behaviorID) {
        return std::find_if(m_behaviors.begin(), m_behaviors.end(), [&](IEntityComponentBehavior* behavior){
                return behavior->Id() == behaviorID;
        });
    }

    ECBStorage& ECBStorage::GetInstance() {
        static ECBStorage storage;
        return storage;
    }
}
