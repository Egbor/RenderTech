#ifndef SCENERENDERSET
#define SCENERENDERSET

#include "Engine/Rendering/RenderSet/RenderSet.h"
#include "Engine/Core/Render/Api/Context.h"

namespace Engine {
	class SceneRenderSet : public RenderSet {
	private:
		Context* m_context;

		Int32 m_viewportWidth;
		Int32 m_viewportHeight;

	public:
		SceneRenderSet(Context* context, Texture2D* target);
		virtual ~SceneRenderSet() = default;

		void SetViewport(Int32 width, Int32 height);

	protected:
		void PreExecute() override;
	};
}

#endif // !SCENERENDERSET
