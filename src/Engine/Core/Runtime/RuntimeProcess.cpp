#include "Engine/Core/Runtime/RuntimeProcess.h"

namespace Engine {
	RuntimeProcess::RuntimeProcess(Int32 id, RuntimeSync* sync, Callback* callback, const RuntimeProcessArgs& args)
		: m_id(id) {
		Launch(sync, callback, args);
	}

	RuntimeProcess::~RuntimeProcess() {
		m_thread.join();
	}

	void RuntimeProcess::Launch(RuntimeSync* sync, Callback* callback, const RuntimeProcessArgs& args) {
		sync->RegisterProcess(this);
		m_thread = std::thread([&, sync, callback, args]() {
			for (RuntimeState state = RuntimeState::RPS_PAUSE; state != RuntimeState::RPS_TERMINATE; state = sync->SyncProcess(this)) {
				if (state == RuntimeState::RPS_PROGRESS) {
					callback->Invoke(args.time->DeltaTime());
				}
			}
			Callback::Free(callback);
		});
	}

	Int32 RuntimeProcess::GetId() const {
		return m_id;
	}

	bool RuntimeProcess::operator==(const RuntimeProcess& process) const {
		return m_thread.get_id() == process.m_thread.get_id();
	}
}