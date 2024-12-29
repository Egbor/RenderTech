#include "Engine/Core/Core.h"

#include <sstream>

namespace Engine {
	Callable<void(const String&)>* _Core::errorMessageBox = nullptr;

	void _Core::InitiateFatalError(const String& tag, const String& message) {
		if (errorMessageBox != nullptr) {
			std::stringstream ss;
			ss << "[" << tag << "]: " << message;

			errorMessageBox->Invoke(ss.str());
		}
		std::abort();
	}

	_Core::_Core(IWindow* window, IContext* context) 
		: m_window(window), m_context(context) {
		errorMessageBox = Delegate<IWindow, void(const String&)>::AllocateDelegate(m_window, &IWindow::InvokeErrorMessageBox);
		m_context->Init(this);
	}

	_Core::~_Core() {
		Callable<void(const String&)>::Free(errorMessageBox);
	}

	const IContext* _Core::GetContext() const {
		return m_context;
	}

	const IWindow* _Core::GetWindow() const {
		return m_window;
	}

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