#ifndef CORE_H
#define CORE_H

#include "Engine/Core/Render/Base/Interface/IContext.h"
#include "Engine/Core/Render/Base/RenderBase.h"

#include "Engine/Core/System/Platform/Interface/IWindow.h"
#include "Engine/Core/System/Platform/Interface/IInputable.h"
#include "Engine/Core/System/Resource/ResourceManager.h"

namespace Engine {
	class Core {
	private:
		Core();
		virtual ~Core() = default;

	public:
		IContext* GetContext() const;
		IWindow* GetWindow() const;
		Input* GetInput() const;
		
		void Initialize(IWindow* window, IContext* context);
		
		static Core* GetInstance();

		template<class TObjectClass>
		static TObjectClass* Load(const String& filename) {
			return core->m_resourceManager->Load<TObjectClass>(filename);
		}

	private:
		static Core* core;

		IContext* m_context;
		IWindow* m_window;

		ResourceManager* m_resourceManager;
	};
}

#endif // !CORE_H
