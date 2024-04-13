#ifndef SCENE_H
#define SCENE_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Component/CameraComponent.h"

#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	class Scene {
	private:
		Array<Entity*> m_entities;

	public:
		Scene();
		virtual ~Scene() = default;

		void InsertEntity(Entity* entity);
		void RemoveEntity(Entity* entity);
		void DoTraversal(std::function<void(Entity*)> callback);
		void DoTraversal(IClass* traversalClass, std::function<void(SceneComponent*)> callback);

		CameraComponent* GetCamera() const;
	};
}

#endif // !SCENE_H
