#ifndef SCENE_H
#define SCENE_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Component/CameraComponent.h"

namespace Engine {
	class Scene {
	public:
		Scene() = default;
		virtual ~Scene() = default;

		void DoTraversal(IClass* traversalClass, std::function<void(SceneComponent*)> callback);
		CameraComponent* GetCamera() const;
	};
}

#endif // !SCENE_H
