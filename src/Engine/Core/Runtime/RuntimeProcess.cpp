#include "Engine/Core/Runtime/RuntimeProcess.h"

namespace Engine {
	RuntimeProcess::RuntimeProcess(Int32 id, RuntimeSync* sync, const Time* time, Callable<void(Float)>* callback)
		: m_id(id) {
		Launch(sync, time, callback);
	}

	RuntimeProcess::~RuntimeProcess() {
		m_thread.join();
	}

	void RuntimeProcess::Launch(RuntimeSync* sync, const Time* time, Callable<void(Float)>* callback) {
		sync->RegisterProcess(this);
		m_thread = std::thread([&, sync, time, callback]() {
			for (RuntimeState state = RuntimeState::RPS_PAUSE; state != RuntimeState::RPS_TERMINATE; state = sync->SyncProcess(this)) {
				if (state == RuntimeState::RPS_PROGRESS) {
					callback->Invoke(time->DeltaTime());
				}
			}
			Callable<void(Float)>::Free(callback);
		});
	}

	Int32 RuntimeProcess::GetId() const {
		return m_id;
	}

	bool RuntimeProcess::operator==(const RuntimeProcess& process) const {
		return m_thread.get_id() == process.m_thread.get_id();
	}
}