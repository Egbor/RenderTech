#include "Engine/Core/Runtime/Runtime.h"
#include "Engine/Core/Runtime/RuntimeProcess.h"

#include <sstream>
#include <Windows.h>

namespace Engine {
#define ALL_SYNC 0xFFFFFFFF

	Runtime::Runtime(IWindow* window, IContext* context)
		: m_core(new _Core(window, context)), m_time(new Time())
		, m_sync(new RuntimeSync()) {
		m_tasks.reserve(RuntimeSync::MAX_PROCESSES_NUMBER);
		m_sync->RegisterSyncCallback(Delegate<Runtime, void()>::AllocateDelegate(this, &Runtime::Sync));
	}

	Runtime::~Runtime() {
		DELETE_OBJECT(m_time);
		DELETE_OBJECT(m_sync);
		DELETE_OBJECT(m_core);
	}

	void Runtime::DelegateProcess(RuntimeProcess::Callback* callback) {
		if (RuntimeSync::MAX_PROCESSES_NUMBER != m_tasks.size()) {
			m_tasks.push_back(new RuntimeProcess(static_cast<Int32>(m_tasks.size()), m_sync, callback, { m_time }));
		}
	}

	void Runtime::Start() {
		m_time->Start();
		m_sync->Set(RuntimeState::RPS_PROGRESS);
	}

	void Runtime::Pause() {
		m_time->Stop();
		m_sync->Set(RuntimeState::RPS_PAUSE);
	}

	void Runtime::Terminate() {
		m_time->Reset();
		m_sync->Set(RuntimeState::RPS_TERMINATE);
	}

	void Runtime::ExitSafty() {
		DELETE_ARRAY_OF_OBJECTS(m_tasks);
	}

	void Runtime::Sync() {
		m_time->Tick();

		std::stringstream ss;
		ss << "---------Sync: " << m_time->DeltaTime() << "(" << std::this_thread::get_id() << ")" << "---------" << std::endl;
		OutputDebugStringA(ss.str().c_str());
	}

	const _Core* Runtime::GetCore() const {
		return m_core;
	}
}