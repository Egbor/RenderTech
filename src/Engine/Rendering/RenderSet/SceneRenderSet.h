#ifndef SCENERENDERSET
#define SCENERENDERSET

#include "Engine/Rendering/RenderSet/RenderSet.h"
#include "Engine/Core/Render/Api/Context.h"

namespace Engine {
	class SceneRenderSet : public RenderSet {
	private:
		Context* m_context;
		Texture2D* m_target;

	public:
		SceneRenderSet(Context* context, Texture2D* target);
		virtual ~SceneRenderSet() = default;

	protected:
		void PreExecute() override;
	};
}

#endif // !SCENERENDERSET
