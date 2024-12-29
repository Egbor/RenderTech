#ifndef CORE_H
#define CORE_H

#include "Engine/Core/Render/Base/Interface/IContext.h"
#include "Engine/Core/Render/Base/RenderBase.h"

#include "Engine/Core/System/Platform/Interface/IWindow.h"
#include "Engine/Core/System/Platform/Interface/IInputable.h"
#include "Engine/Core/System/Resource/ResourceManager.h"

#include "Engine/Core/Utils/Event.h"

#define ENGINE_DEBUG_MESSAGE(msg) __FUNCSIG__, msg

namespace Engine {
	class _Core {
	public:
		_Core(IWindow* window, IContext* context);
		~_Core();

		static void InitiateFatalError(const String& tag, const String& message);

		const IContext* GetContext() const;
		const IWindow* GetWindow() const;

	private:
		IContext* m_context;
		IWindow* m_window;

		static Callable<void(const String&)>* errorMessageBox;
	};

	class Core {
	private:
		Core();
		virtual ~Core();

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
