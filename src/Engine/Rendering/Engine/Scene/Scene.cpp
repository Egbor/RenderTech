#include "Engine/Rendering/Engine/Scene/Scene.h"

namespace Engine {
	Scene::Scene() 
		: m_entities() {
		m_skybox = ClassType<Skybox>::CreateObject(ObjectArgument::Dummy());
	}

	void Scene::InsertEntity(Entity* entity) {
		m_entities.push_back(entity);
	}

	void Scene::RemoveEntity(Entity* entity) {
		auto it = std::find_if(m_entities.begin(), m_entities.end(), [&](Entity* obj) { return entity == obj; });
		if (it != m_entities.end()) {
			m_entities.erase(it);
		}
	}

	void Scene::DoTraversal(std::function<void(Entity*)> callback) {
		for (Int32 i = 0; i < m_entities.size(); i++) {
			callback(m_entities[i]);
		}
	}

	void Scene::DoTraversal(IClass* traversalClass, std::function<void(SceneComponent*)> callback) {
		DoTraversal([&](Entity* entity) {
			Array<SceneComponent*> m_components;
			entity->GetRootComponent()->GetChildrenComponents(m_components);

			for (Int32 i = 0; i < m_components.size(); i++) {
				if (m_components[i]->Is(traversalClass->IClass_GetName())) {
					callback(m_components[i]);
				}
			}
		});
	}

	CameraComponent* Scene::GetCamera() const {
		return CameraComponent::MainCamera();
	}

	Skybox* Scene::GetSkybox() const {
		return m_skybox;
	}
}