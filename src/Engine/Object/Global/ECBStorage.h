#ifndef ECBSTORAGE_H
#define ECBSTORAGE_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Component/Behavior/IEntityComponentBehavior.h"

namespace Engine {
    class ECBStorage {
    private:
        ECBStorage() = default;
        virtual ~ECBStorage() = default;

        Array<IEntityComponentBehavior*> m_behaviors;

    public:
        ECBStorage(const ECBStorage&) = delete;
        void operator=(const ECBStorage&) = delete;

        void AddNewComponentBehavior(IEntityComponentBehavior* behavior);
        void DeleteComponentBehavior(UInt64 behaviorID);

        IEntityComponentBehavior* FindComponentBehavior(UInt64 behaviorID);

        template<class TEntityComponentBehavior>
        TEntityComponentBehavior* FindComponentBehavior(UInt64 behaviorID) {
            return static_cast<TEntityComponentBehavior*>(FindComponentBehavior(behaviorID));
        }

        static ECBStorage& GetInstance();

    private:
        ArrayIterator<IEntityComponentBehavior*> FindComponentBehaviorIterator(UInt64 behaviorID);
    };
}

#endif // ECBSTORAGE_H
