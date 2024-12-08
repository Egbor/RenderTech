#include "Engine/Core/Runtime/RuntimeSync.h"
#include "Engine/Core/Runtime/RuntimeProcess.h"

#include <sstream>
#include <Windows.h>

namespace Engine {
#define ALL_SYNC 0xFFFFFFFF

	RuntimeSync::RuntimeSync() 
		: m_state(RuntimeState::RPS_PAUSE), m_syncs(ALL_SYNC), m_syncsMask(ALL_SYNC)
		, m_cv(), m_mutex() {

	}

	RuntimeState RuntimeSync::SyncProcess(const RuntimeProcess* process) {
		Int32 bitFlag = 1 << process->GetId();
		
		std::unique_lock lock(m_mutex);
		
		m_syncs |= bitFlag;
		if (m_syncs == ALL_SYNC) {
			m_syncEvent();

			m_syncs &= m_syncsMask;
			m_cv.notify_all();
		} else {
			m_cv.wait(lock, [&]() { return !(m_syncs & bitFlag); });
		}

		return m_state;
	}

	void RuntimeSync::RegisterProcess(const RuntimeProcess* process) {
		Int32 bitFlags = ~(1 << process->GetId());

		m_syncsMask &= bitFlags;
		m_syncs &= bitFlags;
	}

	void RuntimeSync::RegisterSyncCallback(Callable<void()>* callback) {
		m_syncEvent += callback;
	}

	void RuntimeSync::Set(RuntimeState state) {
		m_state = state;
	}
}