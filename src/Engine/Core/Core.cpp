#include "Engine/Core/Core.h"

namespace Engine {
	Core* Core::core = new Core();

	Core::Core() 
		: m_window(nullptr), m_context(nullptr) {
		m_resourceManager = new ResourceManager();
	}

	Core::~Core() {
		DELETE_OBJECT(m_resourceManager);
	}

	IContext* Core::GetContext() const {
		return m_context;
	}

	IWindow* Core::GetWindow() const {
		return m_window;
	}

	Input* Core::GetInput() const {
		return dynamic_cast<IInputable*>(m_window)->GetInput();
	}

	void Core::Initialize(IWindow* window, IContext* context) {
		m_window = window;
		m_context = context;
	}

	Core* Core::GetInstance() {
		return core;
	}
}