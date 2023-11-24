#ifndef CORE_H
#define CORE_H

#include "Engine/Core/Render/Api/Interface/IContext.h"
#include "Engine/Core/System/Platform/Interface/IWindow.h"

namespace Engine {
	class Core {
	private:
		Core();
		virtual ~Core() = default;

	public:
		IContext* GetContext() const;
		IWindow* GetWindow() const;
		
		void Initialize(IWindow* window, IContext* context);
		
		static Core* GetInstance();

	private:
		static Core* core;

		IContext* m_context;
		IWindow* m_window;
	};
}

#endif // !CORE_H
